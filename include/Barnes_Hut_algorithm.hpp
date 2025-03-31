#ifndef BARNESHUT_HPP
#define BARNESHUT_HPP

#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"
#include "vector_operator.hpp"

#define MAX_SIZE 10000
#define THETA 0.5

namespace Barnes_Hut_struct {
    
    struct Node {
        
        // <next> is an array that contains the indices of the 4 children node of this node. 
        // In order : next[0] node 1, next[1] node 2, next[2] node 3, next[4] node 4. 
        // In use Euclidean 2D space node notation. 
        int next[4];

        // <center> is the geometric center of the node, wich is a square, with width and height <size>*2. 
        sf::Vector2f center;
        float size;

        sf::Vector2f centerOfMass;
        int mass;
    };
    
    struct Quadtree {

        std::vector<Node> qtree;

        void init(){
            
            Node first_node;
            
            first_node.next[0] = 0;
            first_node.center = {640, 360};
            first_node.size = 1000;
            first_node.centerOfMass = {0,0};
            first_node.mass = 0;
            qtree.insert(qtree.begin() ,first_node);
        }

        void init(sf::Vector2f center, float size){
            
            Node first_node;
            
            first_node.next[0] = 0;
            first_node.center = center;
            first_node.size = size;
            first_node.centerOfMass = {0,0};
            first_node.mass = 0;
            qtree.insert(qtree.begin() ,first_node);
        }

        void subdivide(int node){
            
            int qtree_size = qtree.size();
            float new_node_size = qtree[node].size/2;
            
            // node I
            qtree.insert(qtree.end(), Node());
            qtree[qtree_size].next[0] = 0;
            qtree[qtree_size].mass = 0;
            qtree[qtree_size].centerOfMass = {0,0};
            qtree[qtree_size].center = {(qtree[node].center.x + (qtree[node].size/2)), (qtree[node].center.y - (qtree[node].size/2))};
            qtree[qtree_size].size = new_node_size;
            qtree[node].next[0] = qtree_size;

            // node II
            qtree.insert(qtree.end(), Node());
            qtree[qtree_size +1].next[0] = 0;
            qtree[qtree_size +1].mass = 0;
            qtree[qtree_size +1].centerOfMass = {0,0};
            qtree[qtree_size +1].center = {(qtree[node].center.x - (qtree[node].size/2)), (qtree[node].center.y - (qtree[node].size/2))};
            qtree[qtree_size +1].size = new_node_size;
            qtree[node].next[1] = qtree_size +1;

            // node III
            qtree.insert(qtree.end(), Node());
            qtree[qtree_size +2].next[0] = 0;
            qtree[qtree_size +2].mass = 0;
            qtree[qtree_size +2].centerOfMass = {0,0};
            qtree[qtree_size +2].center = {(qtree[node].center.x - (qtree[node].size/2)), (qtree[node].center.y + (qtree[node].size/2))};
            qtree[qtree_size +2].size = new_node_size;
            qtree[node].next[2] = qtree_size +2;

            // node IV
            qtree.insert(qtree.end(), Node());
            qtree[qtree_size +3].next[0] = 0;
            qtree[qtree_size +3].mass = 0;
            qtree[qtree_size +3].centerOfMass = {0,0};
            qtree[qtree_size +3].center = {(qtree[node].center.x + (qtree[node].size/2)), (qtree[node].center.y + (qtree[node].size/2))};
            qtree[qtree_size +3].size = new_node_size;
            qtree[node].next[3] = qtree_size +3;

            return;
        }
        

        // If the node is empty (empty => sub-nodes are not initialized and mass is 0), insert the body.
        //
        // If node x is an external node (external => sub-nodes are not initialized and mass is not 0), say containing a body named c, 
        // then there are two bodies b and c in the same region. 
        // Subdivide the region further by creating four sub-nodes. 
        // Then, recursively insert both b and c into the appropriate quadrants. 
        // Since b and c may still end up in the same node, there may be several subdivisions during a single insertion. 
        // Finally, update the center-of-mass and total mass of x.
        //
        // If node x is an internal node (internal => sub-nodes are initialized), update the center-of-mass and total mass of x. 
        // Recursively insert the body b in the appropriate node.
        void insert(int mass, sf::Vector2f pos, int node){

            if(qtree[node].next[0] == 0){
                
                if(qtree[node].mass == 0){

                    qtree[node].centerOfMass = pos;
                    qtree[node].mass = mass;
                    return;
                
                }else{
                
                    subdivide(node);
                    for(int i=qtree[node].next[0]; i < (qtree[node].next[0]+4); ++i){

                        if(abs(qtree[i].center.x - pos.x) <= qtree[i].size && abs(qtree[i].center.y - pos.y) <= qtree[i].size){
                            
                            insert(mass, pos, i);
                            insert(qtree[node].mass, qtree[node].centerOfMass, i);
                            break;
                        }
                    }
                    qtree[node].centerOfMass = (qtree[node].centerOfMass*qtree[node].mass + pos*mass) / (qtree[node].mass + mass);
                    qtree[node].mass += mass;
                    return;

                }

            }else{

                qtree[node].centerOfMass = (qtree[node].centerOfMass*qtree[node].mass + pos*mass) / (qtree[node].mass + mass);
                qtree[node].mass += mass;

                for(int i=qtree[node].next[0]; i < (qtree[node].next[0]+4); ++i){

                    if(abs(qtree[i].center.x - pos.x) <= qtree[i].size && abs(qtree[i].center.y - pos.y) <= qtree[i].size){
                        
                        insert(mass, pos, i);
                        return;

                    }
                }

            }

        }


        sf::Vector2f update_acceleration(int mass, sf::Vector2f pos, int node){
            
            // If the current node is an external node (and it is not body b), 
            // calculate the force exerted by the current node on b, and add this amount to b’s net force.
            if(qtree[node].next[0] == 0){
                
                if(qtree[node].centerOfMass == pos){
                
                    return {0.f,0.f};

                }else{

                    // sf::Vector2f direction = qtree[node].centerOfMass - pos;
                    // float magnitude_sq = direction.x*direction.x + direction.y*direction.y;
                    float magnitude_sq = (qtree[node].centerOfMass - pos).x*(qtree[node].centerOfMass - pos).x + (qtree[node].centerOfMass - pos).y*(qtree[node].centerOfMass - pos).y;
                    float magnitude = sqrt(magnitude_sq);
                    // std::cout << qtree[node].mass << std::endl;
                    return (qtree[node].centerOfMass - pos) * (qtree[node].mass/(magnitude * magnitude_sq));
                }
            }

            // Otherwise, calculate the ratio s/d. (s = width of the node, d = distance between body and center of mass of node)
            // If s/d < θ , treat this internal node as a single body, and calculate the force it exerts on body b, and add this amount to b’s net force.
            // Otherwise, run the procedure recursively on each of the current node’s children.
            if((qtree[node].size*2)/(pos - qtree[node].centerOfMass).length() < THETA){

                // sf::Vector2f direction = qtree[node].centerOfMass - pos;
                // float magnitude_sq = direction.x*direction.x + direction.y*direction.y;
                float magnitude_sq = (qtree[node].centerOfMass - pos).x*(qtree[node].centerOfMass - pos).x + (qtree[node].centerOfMass - pos).y*(qtree[node].centerOfMass - pos).y;
                float magnitude = sqrt(magnitude_sq);
                return (qtree[node].centerOfMass - pos) * (qtree[node].mass/(magnitude * magnitude_sq));
            
            }else{

                return update_acceleration(mass, pos, qtree[node].next[0]) + update_acceleration(mass, pos, qtree[node].next[1]) + update_acceleration(mass, pos, qtree[node].next[2]) + update_acceleration(mass, pos, qtree[node].next[3]);
            }

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
        // q.init({640, 360}, 1000);

        // std::cout << "check point 1" << std::endl;
        for(int i=0; i < GALAXY_DIMENSION; ++i){
            q.insert(galaxy[i].mass, galaxy[i].position, 0);
        }

        // for(int i=0; i < q.qtree.size(); ++i){
        //     std::cout << i << " " << q.qtree[i].mass << " " << q.qtree[i].centerOfMass.x << " " << q.qtree[i].centerOfMass.y << std::endl;
        // }

        // sleep(1000);

        for(int i=0; i < GALAXY_DIMENSION; ++i){
            galaxy[i].acceleration = q.update_acceleration(galaxy[i].mass, galaxy[i].position, 0);
            // std::cout << galaxy[i].acceleration.x << " " << galaxy[i].acceleration.y << std::endl;
        }

    }
}

#endif // BARNESHUT_HPP