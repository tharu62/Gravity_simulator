#ifndef BARNESHUT_HPP
#define BARNESHUT_HPP

#include <iostream>
#include <vector>
#include <stack>
#include <unistd.h>
#include "SFML/Graphics.hpp"
#include "vector_operator.hpp"

#define MAX_SIZE 4000.f
#define THETA 0.5

namespace Barnes_Hut_struct {
    
    /**
     * @param next is an array that contains the indices of the 4 node children of this node.
     * In order : next[0] quadrant 1, next[1] quadrant 2, next[2] quadrant 3, next[4] quadrant 4. 
     * In use Euclidean 2D space quadrant notation. 
     * @param center is the geometric center of the node, wich is a square, with width and height <size>*2.
     * @param CenterOfMass is the center of mass of the node that has total mass of <mass>.   
     */
    struct Node {
        
        int next[4] = {0, 0, 0, 0};

        sf::Vector2f center;
        float size;

        sf::Vector2f centerOfMass;
        unsigned long int mass = 0;

    };
    
    /**
     * @brief
     * @param qtree
     * @param stack 
     */
    struct Quadtree {

        std::vector<Node> qtree;
        std::stack<int> stack;

        void init(){
            qtree.insert(qtree.begin() , Node());
            qtree[0].center = {640, 360};
            qtree[0].size = MAX_SIZE;
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
            qtree[qtree_size].center = {(qtree[node].center.x + new_node_size), (qtree[node].center.y - new_node_size)};
            qtree[qtree_size].size = new_node_size;
            qtree[node].next[0] = qtree_size;

            // node II
            qtree.insert(qtree.end(), Node());
            qtree[qtree_size +1].center = {(qtree[node].center.x - new_node_size), (qtree[node].center.y - new_node_size)};
            qtree[qtree_size +1].size = new_node_size;
            qtree[node].next[1] = qtree_size +1;

            // node III
            qtree.insert(qtree.end(), Node());
            qtree[qtree_size +2].center = {(qtree[node].center.x - new_node_size), (qtree[node].center.y + new_node_size)};
            qtree[qtree_size +2].size = new_node_size;
            qtree[node].next[2] = qtree_size +2;

            // node IV
            qtree.insert(qtree.end(), Node());
            qtree[qtree_size +3].center = {(qtree[node].center.x + new_node_size), (qtree[node].center.y + new_node_size)};
            qtree[qtree_size +3].size = new_node_size;
            qtree[node].next[3] = qtree_size +3;

        }

        /**
         * @brief Insert ONE body mass and position in the quadtree using itarative method.
         */
        void insert(int mass, sf::Vector2f pos){

            for(int i=0; i<qtree.size(); ++i){

                if(abs(qtree[i].center.x - pos.x) <= qtree[i].size && abs(qtree[i].center.y - pos.y) <= qtree[i].size){
                    if(qtree[i].mass == 0){
                        
                        qtree[i].centerOfMass = pos;
                        qtree[i].mass = mass;
                        return;

                    }else{

                        subdivide(i);
                        for(int j = qtree[i].next[0]; j < (qtree[i].next[0]+4); ++j){
                            if(abs(qtree[j].center.x - qtree[i].centerOfMass.x) <= qtree[j].size && abs(qtree[j].center.y - qtree[i].centerOfMass.y) <= qtree[j].size){
                                qtree[j].centerOfMass = qtree[i].centerOfMass;
                                qtree[j].mass = qtree[i].mass;
                                continue;
                            }
                        }
                        qtree[i].centerOfMass = (qtree[i].centerOfMass*qtree[i].mass + pos*mass) / (qtree[i].mass + mass);
                        qtree[i].mass += mass;
                        i = qtree[i].next[0] -1;
                    }
                } 
                
            }

        }

        /**
         * * @brief Returns the acceleration of a body in the quadtree using recursive method.
         */
        sf::Vector2f update_acceleration(sf::Vector2f pos, int i){

            if(abs(pos.x) > MAX_SIZE*2 || abs(pos.y) > MAX_SIZE*2){
                // std::cout << "Position out of bounds!" << std::endl;
                return {0.f, 0.f};
            }
            if(qtree[i].centerOfMass != pos){

                if((qtree[i].size*2)/abs((qtree[i].centerOfMass - pos).length()) < THETA || qtree[i].next[0] == 0){

                    float magnitude_sq = (qtree[i].centerOfMass - pos).x*(qtree[i].centerOfMass - pos).x + (qtree[i].centerOfMass - pos).y*(qtree[i].centerOfMass - pos).y;
                    if(magnitude_sq >= 100.f){
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

        /**
         * @brief Returns the acceleration of a body in the quadtree using iterative method.
         */
        sf::Vector2f update_acceleration(sf::Vector2f pos){

            if(abs(pos.x) > MAX_SIZE*2 || abs(pos.y) > MAX_SIZE*2){
                return {0.f, 0.f};
            }
            
            stack.push(0);
            sf::Vector2f acc = {0,0};
            int i;

            while(!stack.empty()){
                i = stack.top();
                if(qtree[i].centerOfMass != pos){

                    if((qtree[i].size*2)/abs((qtree[i].centerOfMass - pos).length()) < THETA || qtree[i].next[0] == 0){
    
                        float magnitude_sq = (qtree[i].centerOfMass - pos).x*(qtree[i].centerOfMass - pos).x + (qtree[i].centerOfMass - pos).y*(qtree[i].centerOfMass - pos).y;
                        if(magnitude_sq > 0.1f){
                            float magnitude = sqrt(magnitude_sq);
                            acc += ((qtree[i].centerOfMass - pos) * (qtree[i].mass/(magnitude * magnitude_sq)));
                        }
                        stack.pop();
    
                    }else{
                        stack.pop();
                        stack.push(qtree[i].next[0]);
                        stack.push(qtree[i].next[1]);
                        stack.push(qtree[i].next[2]);
                        stack.push(qtree[i].next[3]);
                    }
    
                }else{
                    stack.pop();
                }
                    
            }
            return acc;

        }

        /**
         * @brief Insert ALL bodies mass and position in the quadtree using iterative method.
         */
        void insert(Celestial_body *galaxy){

            int mass;
            sf::Vector2f pos;

            for(int k=0; k<GALAXY_DIMENSION; ++k){

                pos = galaxy[k].position;
                
                if(abs(pos.x) <= MAX_SIZE*2 || abs(pos.y) <= MAX_SIZE*2){
                    
                    mass = galaxy[k].mass;
                    for(int i=0; i<qtree.size(); ++i){
        
                        if(abs(qtree[i].center.x - pos.x) <= qtree[i].size && abs(qtree[i].center.y - pos.y) <= qtree[i].size){
                            if(qtree[i].mass == 0 && qtree[i].centerOfMass == sf::Vector2f(0.f, 0.f)){
                                
                                qtree[i].centerOfMass = pos;
                                qtree[i].mass = mass;
                                continue;
        
                            }else{
        
                                subdivide(i);
                                for(int j = qtree[i].next[0]; j < (qtree[i].next[0]+4); ++j){
                                    if(abs(qtree[j].center.x - qtree[i].centerOfMass.x) <= qtree[j].size && abs(qtree[j].center.y - qtree[i].centerOfMass.y) <= qtree[j].size){
                                        qtree[j].centerOfMass = qtree[i].centerOfMass;
                                        qtree[j].mass = qtree[i].mass;
                                        continue;
                                    }
                                }
                                qtree[i].centerOfMass = (qtree[i].centerOfMass*qtree[i].mass + pos*mass) / (qtree[i].mass + mass);
                                qtree[i].mass += mass;
                                i = qtree[i].next[0] -1;
                            }
                        } 
                        
                    }
                }
    
            }

        }

        /**
         * @brief Updates the acceleration of ALL bodies in the quadtree using iterative method.
         */
        void update_acceleration(Celestial_body *galaxy){

            int i;
            sf::Vector2f pos;
            sf::Vector2f acc;
            
            for(int j=0; j<GALAXY_DIMENSION; ++j){
                
                pos = galaxy[j].position;
                
                if(abs(pos.x) <= MAX_SIZE*2 || abs(pos.y) <= MAX_SIZE*2){

                    acc = {0,0};
                    stack.push(0);
                    while(!stack.empty()){
                        i = stack.top();
                        if(qtree[i].centerOfMass != pos){
        
                            if((qtree[i].size*2)/abs((qtree[i].centerOfMass - pos).length()) < THETA || qtree[i].next[0] == 0){
            
                                float magnitude_sq = (qtree[i].centerOfMass - pos).x*(qtree[i].centerOfMass - pos).x + (qtree[i].centerOfMass - pos).y*(qtree[i].centerOfMass - pos).y;
                                if(magnitude_sq > 0.1f){
                                    float magnitude = sqrt(magnitude_sq);
                                    acc += ((qtree[i].centerOfMass - pos) * (qtree[i].mass/(magnitude * magnitude_sq)));
                                }
                                stack.pop();
            
                            }else{
                                stack.pop();
                                stack.push(qtree[i].next[0]);
                                stack.push(qtree[i].next[1]);
                                stack.push(qtree[i].next[2]);
                                stack.push(qtree[i].next[3]);
                            }
            
                        }else{
                            stack.pop();
                        }
                            
                    }
                    
                    galaxy[j].acceleration = acc;

                }else{

                    galaxy[j].acceleration = {0,0};

                }


            }

        }


        /**
         * @brief Insertion method used for testing purposes.
         */
        void simple_insert(int mass, sf::Vector2f pos){
            if(qtree[0].mass != 0){
                    
                qtree[0].centerOfMass = (qtree[0].centerOfMass*qtree[0].mass + pos*mass) / (qtree[0].mass + mass);

                // std::cout << (qtree[0].centerOfMass*qtree[0].mass).x << " " << (qtree[0].centerOfMass*qtree[0].mass).y << std::endl;
                // if(qtree[0].centerOfMass.x < 0 || qtree[0].centerOfMass.y < 0){
                //     exit(1);
                // }
                
                qtree[0].mass += mass;
                return;

            }else{

                qtree[0].centerOfMass = pos;
                qtree[0].mass = mass;
                return;

            }
        }

        /**
         * @brief Update accelleration method used for testing purposes.
         */
        sf::Vector2f simple_update_acceleration(int mass, sf::Vector2f pos){

            if(pos == qtree[0].centerOfMass) return {0.f, 0.f};
            float magnitude_sq = (qtree[0].centerOfMass - pos).x*(qtree[0].centerOfMass - pos).x + (qtree[0].centerOfMass - pos).y*(qtree[0].centerOfMass - pos).y;
            if(magnitude_sq >= 100.f){
                float magnitude = sqrt(magnitude_sq);
                return (qtree[0].centerOfMass - pos) * (qtree[0].mass/(magnitude_sq * magnitude));
            }

            return {0.f, 0.f};
        }

    };
    
}

namespace Burnes_Hut{

    using namespace Barnes_Hut_struct;

    /**
     * @brief Computes the Gravitational forces between each celestial body to update the acceleration of each celestial body.
     */
    void compute_forces(Celestial_body *galaxy, Quadtree &q){
        
        q.qtree.clear();
        q.init();

        // clock_t start = clock();
        
        for(int i=0; i < GALAXY_DIMENSION; ++i){

            if(abs(galaxy[i].position.x) <= MAX_SIZE*2 && abs(galaxy[i].position.y) <= MAX_SIZE*2){
                q.insert(galaxy[i].mass, galaxy[i].position);
            }
        }
        
        // clock_t end = clock();
        // double elapsed = double(end - start)/CLOCKS_PER_SEC;
        // std::cout << elapsed << std::endl;
        
        for(int i=0; i < GALAXY_DIMENSION; ++i){
            galaxy[i].acceleration = q.update_acceleration(galaxy[i].position);
        } 
        
    }

}

#endif // BARNESHUT_HPP