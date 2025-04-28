#ifndef BARNESHUT_HPP
#define BARNESHUT_HPP

#include <iostream>
#include <vector>
#include <stack>
#include <unistd.h>
#include "SFML/Graphics.hpp"
#include "vector_operator.hpp"

#define MAX_SIZE 4000.f
#define THETA 0.1f
#define LINUX_SCALE_FACTOR 0.25

extern int GALAXY_DIMENSION;

namespace Barnes_Hut_struct {
    
    /**
     * @param next is an array that contains the indices of the 4 node children of this node.
     * In order : next[0] quadrant 1, next[1] quadrant 2, next[2] quadrant 3, next[4] quadrant 4. 
     * In use Euclidean 2D space quadrant notation. 
     * @param center is the geometric center of the node, wich is a square, with width and height <size>*2.
     * @param CenterOfMass is the center of mass of the node that has total mass of <mass>.   
     * @details total size of struct = 8*4 + 4*2 + 8*2 = 56 bytes.
     */
    struct Node {
        
        u_int32_t next[4];  //8x4 bytes
        float size;         //4 bytes
        float mass;         //4 bytes

        sf::Vector2f center;        //8 bytes
        sf::Vector2f centerOfMass;  //8 bytes

        Node() : next{0,0,0,0}, center(0.f, 0.f), size(0.f), centerOfMass(0.f, 0.f), mass(0) {}

    };
    
    /**
     * @brief A Quadtree is a tree data structure in which each node, apart from the leafs, has four children. 
     * @param qtree is the vector that contains the Nodes of the quadtree.
     * @param stack is the Stack used in the iterative method for updating acceleration.
     */
    struct Quadtree {

        std::vector<Node> qtree;
        std::stack<u_int32_t> stack;

        /**
         * @brief Initializes the quadtree with the root node and reserves space for the quadtree vector with a size of GALAXY_DIMENSION*2*log2(GALAXY_DIMENSION).
         */
        void init(){
            qtree.reserve(GALAXY_DIMENSION * 2 *log2(GALAXY_DIMENSION));
            qtree.insert(qtree.begin() , Node());
            qtree[0].center = {640, 360};
            qtree[0].size = MAX_SIZE;
        }

        /**
         * @brief Subdivides the given node into 4 children nodes.
         */
        void subdivide(u_int32_t node){
            
            if (node < 0 || node >= qtree.size()) {
                std::cerr << "Error: Node index out of bounds in subdivide()." << std::endl;
                return;
            }

            u_int32_t qtree_size = qtree.size();
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
        void insert(float mass, sf::Vector2f pos){

            for(u_int i=0; i<qtree.size(); ++i){

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
        sf::Vector2f update_acceleration(sf::Vector2f pos, u_int32_t i){

            if(abs(pos.x) > MAX_SIZE*2 || abs(pos.y) > MAX_SIZE*2){
                // std::cout << "Position out of bounds!" << std::endl;
                return {0.f, 0.f};
            }
            if(qtree[i].centerOfMass != pos){

                if((qtree[i].size*2)/abs((qtree[i].centerOfMass - pos).length()) < THETA || qtree[i].next[0] == 0){

                    float magnitude_sq = (qtree[i].centerOfMass - pos).x*(qtree[i].centerOfMass - pos).x + (qtree[i].centerOfMass - pos).y*(qtree[i].centerOfMass - pos).y;
                    if(magnitude_sq >= 0.1f){
                        float magnitude = sqrt(magnitude_sq);
                        return ((qtree[i].centerOfMass - pos) * (qtree[i].mass/(magnitude * magnitude_sq)) * LINUX_SCALE_FACTOR);
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
            u_int32_t i;
            float magnitude_sq = 0.f; 
            float magnitude = 0.f;
            sf::Vector2f acc = {0,0};


            while(!stack.empty()){
                i = stack.top();
                if(qtree[i].centerOfMass != pos){
                    
                    if((qtree[i].size*2)/abs((qtree[i].centerOfMass - pos).length()) < THETA || qtree[i].next[0] == 0){  
                        magnitude_sq = (qtree[i].centerOfMass - pos).x*(qtree[i].centerOfMass - pos).x + (qtree[i].centerOfMass - pos).y*(qtree[i].centerOfMass - pos).y;
                        if(magnitude_sq > 0.1f){
                            magnitude = sqrt(magnitude_sq);
                            acc += ((qtree[i].centerOfMass - pos) * (qtree[i].mass/(magnitude * magnitude_sq))) * LINUX_SCALE_FACTOR;
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

            // float mass;
            // sf::Vector2f pos;

            for(u_int32_t k=0; k<GALAXY_DIMENSION; ++k){

                // pos = galaxy[k].position;
                
                // if(abs(pos.x) <= MAX_SIZE*2 || abs(pos.y) <= MAX_SIZE*2){
                if(abs(galaxy[k].position.x) <= MAX_SIZE*2 || abs(galaxy[k].position.y) <= MAX_SIZE*2){
                    
                    // mass = galaxy[k].mass;

                    for(u_int32_t i=0; i<qtree.size(); ++i){
        
                        // if(abs(qtree[i].center.x - pos.x) <= qtree[i].size && abs(qtree[i].center.y - pos.y) <= qtree[i].size){
                        if(abs(qtree[i].center.x - galaxy[k].position.x) <= qtree[i].size && abs(qtree[i].center.y - galaxy[k].position.y) <= qtree[i].size){    
                            if(qtree[i].mass == 0 && qtree[i].centerOfMass == sf::Vector2f(0.f, 0.f)){
                                
                                qtree[i].centerOfMass = galaxy[k].position;
                                // qtree[i].mass = mass;
                                qtree[i].mass = galaxy[k].mass;
                                continue;
        
                            }else{
        
                                subdivide(i);
                                for(u_int32_t j = qtree[i].next[0]; j < (qtree[i].next[0]+4); ++j){
                                    if(abs(qtree[j].center.x - qtree[i].centerOfMass.x) <= qtree[j].size && abs(qtree[j].center.y - qtree[i].centerOfMass.y) <= qtree[j].size){
                                        qtree[j].centerOfMass = qtree[i].centerOfMass;
                                        qtree[j].mass = qtree[i].mass;
                                        continue;
                                    }
                                }
                                // qtree[i].centerOfMass = (qtree[i].centerOfMass*qtree[i].mass + pos*mass) / (qtree[i].mass + mass);
                                qtree[i].centerOfMass = (qtree[i].centerOfMass*qtree[i].mass + galaxy[k].position*galaxy[k].mass) / (qtree[i].mass + galaxy[k].mass);
                                // qtree[i].mass += mass;
                                qtree[i].mass += galaxy[k].mass;
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

            u_int32_t i;
            sf::Vector2f pos;
            sf::Vector2f acc;
            
            for(u_int32_t j=0; j<GALAXY_DIMENSION; ++j){
                
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
                                    acc += ((qtree[i].centerOfMass - pos) * (qtree[i].mass/(magnitude * magnitude_sq)) * LINUX_SCALE_FACTOR);
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
        void simple_insert(float mass, sf::Vector2f pos){
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
        sf::Vector2f simple_update_acceleration(float mass, sf::Vector2f pos){

            if(pos == qtree[0].centerOfMass) return {0.f, 0.f};
            float magnitude_sq = (qtree[0].centerOfMass - pos).x*(qtree[0].centerOfMass - pos).x + (qtree[0].centerOfMass - pos).y*(qtree[0].centerOfMass - pos).y;
            if(magnitude_sq >= 0.1f){
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
        while (!q.stack.empty()) q.stack.pop();
        q.init();

        // q.insert(galaxy);
        
        for(int i=0; i < GALAXY_DIMENSION; ++i){    
            if(abs(galaxy[i].position.x) <= MAX_SIZE*2 && abs(galaxy[i].position.y) <= MAX_SIZE*2){
                q.insert(galaxy[i].mass, galaxy[i].position);
                // q.simple_insert(galaxy[i].mass, galaxy[i].position);
            }
        }
        
        // q.update_acceleration(galaxy);

        for(int i=0; i < GALAXY_DIMENSION; ++i){
            // galaxy[i].acceleration = q.update_acceleration(galaxy[i].position);
            galaxy[i].acceleration = q.simple_update_acceleration(galaxy[i].mass, galaxy[i].position);
        } 

        // clock_t start = clock();
        // clock_t end = clock();
        // double elapsed = double(end - start)/CLOCKS_PER_SEC;
        // std::cout << elapsed << std::endl;
    }

}

#endif // BARNESHUT_HPP