#ifndef BARNESHUT_HPP
#define BARNESHUT_HPP

#include <iostream>
#include <vector>
#include <unistd.h>
#include "SFML/Graphics.hpp"
#include "vector_operator.hpp"

#define MAX_SIZE 2000
#define THETA 0.5

namespace Barnes_Hut_struct {
    
    struct Node {
        
        // <next> is an array that contains the indices of the 4 children node of this node. 
        // In order : next[0] node 1, next[1] node 2, next[2] node 3, next[4] node 4. 
        // In use Euclidean 2D space node notation. 
        int next[4] = {0, 0, 0, 0};

        // <center> is the geometric center of the node, wich is a square, with width and height <size>*2. 
        sf::Vector2f center;
        float size;

        sf::Vector2f centerOfMass;
        int mass = 0;

    };
    
    struct Quadtree {

        std::vector<Node> qtree;
        bool initialized = false;

        void init(){
            
            qtree.insert(qtree.begin() , Node());
            qtree[0].center = {640, 360};
            qtree[0].size = MAX_SIZE;
            initialized = true;

        }

        void clear(){
            for(int i=0; i < qtree.size(); ++i){
                qtree[i].mass = 0;
                qtree[i].centerOfMass = {0.f, 0.f};
            }
        }

        void subdivide(int node){
            
            int qtree_size = qtree.size();
            float new_node_size = qtree[node].size/2;

            // node I
            qtree.insert(qtree.end(), Node());
            qtree[qtree_size].center = {(qtree[node].center.x + (qtree[node].size/2)), (qtree[node].center.y - (qtree[node].size/2))};
            qtree[qtree_size].size = new_node_size;
            qtree[node].next[0] = qtree_size;

            // node II
            qtree.insert(qtree.end(), Node());
            qtree[qtree_size +1].center = {(qtree[node].center.x - (qtree[node].size/2)), (qtree[node].center.y - (qtree[node].size/2))};
            qtree[qtree_size +1].size = new_node_size;
            qtree[node].next[1] = qtree_size +1;

            // node III
            qtree.insert(qtree.end(), Node());
            qtree[qtree_size +2].center = {(qtree[node].center.x - (qtree[node].size/2)), (qtree[node].center.y + (qtree[node].size/2))};
            qtree[qtree_size +2].size = new_node_size;
            qtree[node].next[2] = qtree_size +2;

            // node IV
            qtree.insert(qtree.end(), Node());
            qtree[qtree_size +3].center = {(qtree[node].center.x + (qtree[node].size/2)), (qtree[node].center.y + (qtree[node].size/2))};
            qtree[qtree_size +3].size = new_node_size;
            qtree[node].next[3] = qtree_size +3;

        }

        void insert(int mass, sf::Vector2f pos){

            if(abs(pos.x) > MAX_SIZE || abs(pos.y) > MAX_SIZE){
                // std::cout << "Position out of bounds!" << std::endl;
                return;
            }

            for(int i=0; i<qtree.size(); ++i){

                if(abs(qtree[i].center.x - pos.x) <= qtree[i].size && abs(qtree[i].center.y - pos.y) <= qtree[i].size){
                    if(qtree[i].mass == 0 && qtree[i].centerOfMass == sf::Vector2f(0.f, 0.f)){
                        
                        qtree[i].centerOfMass = pos;
                        qtree[i].mass = mass;
                        return;

                    }else{

                        if(qtree[i].next[0] == 0){
                            subdivide(i);
                        }
                        for(int j = qtree[i].next[0]; j < (qtree[i].next[0]+4); ++j){
                            if(abs(qtree[j].center.x - qtree[i].centerOfMass.x) <= qtree[j].size && abs(qtree[j].center.y - qtree[i].centerOfMass.y) <= qtree[j].size){
                                qtree[j].centerOfMass = qtree[i].centerOfMass;
                                qtree[j].mass = qtree[i].mass;
                                break;
                            }
                        }
                        qtree[i].centerOfMass = (qtree[i].centerOfMass*qtree[i].mass + pos*mass) / (qtree[i].mass + mass);
                        qtree[i].mass += mass;
                        i = qtree[i].next[0] -1;
                    }
                } 
                
            }

        }

        sf::Vector2f update_acceleration(sf::Vector2f pos, int i){

            if(abs(pos.x) > MAX_SIZE || abs(pos.y) > MAX_SIZE){
                // std::cout << "Position out of bounds!" << std::endl;
                return {0.f, 0.f};
            }
            if(qtree[i].centerOfMass != pos){

                if((qtree[i].size*2)/abs((qtree[i].centerOfMass - pos).length()) < THETA || qtree[i].next[0] == 0){

                    float magnitude_sq = (qtree[i].centerOfMass - pos).x*(qtree[i].centerOfMass - pos).x + (qtree[i].centerOfMass - pos).y*(qtree[i].centerOfMass - pos).y;
                    if(magnitude_sq >= 2.f){
                        float magnitude = sqrt(magnitude_sq);
                        return ((qtree[i].centerOfMass - pos) * (qtree[i].mass/(magnitude * magnitude_sq)));
                    }

                }else{

                    return update_acceleration(pos, qtree[i].next[0]) + 
                           update_acceleration(pos, qtree[i].next[1]) +
                           update_acceleration(pos, qtree[i].next[2]) +
                           update_acceleration(pos, qtree[i].next[3]);
                }

            }
            return {0.f, 0.f};

        }

        void simple_insert(int mass, sf::Vector2f pos){
            if(qtree[0].mass != 0){
                    
                qtree[0].centerOfMass = (qtree[0].centerOfMass*qtree[0].mass + pos*mass) / (qtree[0].mass + mass);
                qtree[0].mass += mass;
                return;

            }else{

                qtree[0].centerOfMass = pos;
                qtree[0].mass = mass;
                return;

            }
        }

        sf::Vector2f simple_update_acceleration(int mass, sf::Vector2f pos){

            if(pos == qtree[0].centerOfMass) return {0.f, 0.f};
            float magnitude_sq = (qtree[0].centerOfMass - pos).x*(qtree[0].centerOfMass - pos).x + (qtree[0].centerOfMass - pos).y*(qtree[0].centerOfMass - pos).y;
            if(magnitude_sq >= 2.f){
                float magnitude = sqrt(magnitude_sq);
                return (qtree[0].centerOfMass - pos) * (qtree[0].mass/(magnitude * magnitude_sq));
            }

            return {0.f, 0.f};
        }

    };
    
}

namespace Burnes_Hut{

    using namespace Barnes_Hut_struct;

    /**
     * @brief 
     */
    void compute_forces(Celestial_body *galaxy, Quadtree &q){

        if(q.initialized){
            q.clear();
        }else{
            q.init();
        }

        for(int i=0; i < GALAXY_DIMENSION; ++i){
            q.insert(galaxy[i].mass, galaxy[i].position);
            // q.simple_insert(galaxy[i].mass, galaxy[i].position);
        }
        
        // for(int i=0; i < q.qtree.size(); ++i){
        // }
        
        // std::cout << q.qtree[0].centerOfMass.x << " " << q.qtree[0].centerOfMass.y << std::endl;
        // exit(0);
        
        for(int i=0; i < GALAXY_DIMENSION; ++i){
            // galaxy[i].acceleration = q.simple_update_acceleration(galaxy[i].mass, galaxy[i].position);
            galaxy[i].acceleration =  q.update_acceleration(galaxy[i].position, 0);
        } 
    }

}

#endif // BARNESHUT_HPP