#ifndef BARNESHUT_HPP
#define BARNESHUT_HPP

#include <iostream>
#include <vector>
#include <unistd.h>
#include "SFML/Graphics.hpp"
#include "vector_operator.hpp"

#define MAX_SIZE 10000
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

        void init(){
            
            qtree.clear();
            qtree.insert(qtree.begin() , Node());
            qtree[0].next[0] = 0;
            qtree[0].center = {640, 360};
            qtree[0].size = MAX_SIZE;

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

        int find_quadrant(sf::Vector2f pos){
            for(int i=0; i<qtree.size(); ++i){
                if(abs(qtree[i].center.x - pos.x) <= qtree[i].size && abs(qtree[i].center.y - pos.y) <= qtree[i].size){
                    if(qtree[i].next[0] == 0){
                        return i;
                    }else{
                        i = qtree[i].next[0] - 1;
                    }
                }
            }
            return -1;
        }

        int find_quadrant_from(int i, sf::Vector2f pos){
            for(int k=0; k<4; ++k){
                if(abs(qtree[qtree[i].next[k]].center.x - pos.x) <= qtree[qtree[i].next[k]].size && abs(qtree[qtree[i].next[k]].center.y - pos.y) <= qtree[qtree[i].next[k]].size){
                    return qtree[i].next[k];
                }
            }
            return -1;
        }
    
        void insert(int mass, sf::Vector2f pos){
            
            int i = find_quadrant(pos);
            int j = 0;
            while(true){

                if(i == -1){
                    // std::cout << "object out of bound, mass: " << mass << " pos: "<< pos.x << " " << pos.y << std::endl;
                    // exit(-1);
                    return;
                }

                if(qtree[i].mass != 0){
                    
                    subdivide(i);
                    j = find_quadrant_from(i, qtree[i].centerOfMass);
                    qtree[j].centerOfMass = qtree[i].centerOfMass;
                    qtree[j].mass = qtree[i].mass;
                    qtree[i].centerOfMass = (qtree[i].centerOfMass*qtree[i].mass + pos*mass) / (qtree[i].mass + mass);
                    qtree[i].mass += mass;
                    i = find_quadrant_from(i, pos);

                }else{

                    qtree[i].centerOfMass = pos;
                    qtree[i].mass = mass;
                    return;

                }
            }
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

            sf::Vector2f acc = {0.f, 0.f}; 
            if(pos == qtree[0].centerOfMass) return acc;
            float magnitude_sq = (qtree[0].centerOfMass - pos).x*(qtree[0].centerOfMass - pos).x + (qtree[0].centerOfMass - pos).y*(qtree[0].centerOfMass - pos).y;
            if(magnitude_sq >= 2.f){
                float magnitude = sqrt(magnitude_sq);
                acc = (qtree[0].centerOfMass - pos) * (qtree[0].mass/(magnitude * magnitude_sq));
            }

            return acc;
        }

    };
    
}

namespace Burnes_Hut{

    using namespace Barnes_Hut_struct;

    /**
     * @brief 
     */
    void compute_forces(Celestial_body *galaxy){
        
        Quadtree q;
        q.init();

        for(int i=0; i < GALAXY_DIMENSION; ++i){
            q.simple_insert(galaxy[i].mass, galaxy[i].position);
            // q.insert(galaxy[i].mass, galaxy[i].position);
        }
        
        for(int i=0; i < GALAXY_DIMENSION; ++i){
            galaxy[i].acceleration = q.simple_update_acceleration(galaxy[i].mass, galaxy[i].position);
        } 
            
    }

}

#endif // BARNESHUT_HPP