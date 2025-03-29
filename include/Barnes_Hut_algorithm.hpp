#ifndef BARNESHUT_HPP
#define BARNESHUT_HPP

#include <iostream>
#include "SFML/Graphics.hpp"
#include "vector_operator.hpp"

#define MAX_SIZE 1000

namespace Barnes_Hut_struct {

    struct Quadrant {
        
        sf::Vector2f center;
        float size;

    };
    
    struct Node {
        
        Node *next[4]; 
        Quadrant quad;
        sf::Vector2f centerOfMass;
        int mass;
    };
    
    struct Quadtree {

        std::vector<Node> qtree;

        void subdivide(int quadrant){
            
            qtree[quadrant].mass = 0;
            qtree[quadrant].quad.center = {qtree[quadrant].quad.center.x, qtree[quadrant].quad.center.y};
            
            qtree[quadrant+1].mass = 0;
            qtree[quadrant+1].quad.center = {qtree[quadrant].quad.center.x, qtree[quadrant].quad.center.y};
            
            qtree[quadrant+2].mass = 0;
            qtree[quadrant+2].quad.center = {qtree[quadrant].quad.center.x, qtree[quadrant].quad.center.y};
            
            qtree[quadrant+3].mass = 0;
            qtree[quadrant+3].quad.center = {qtree[quadrant].quad.center.x, qtree[quadrant].quad.center.y};
        }
        
        void insert(int &mass, sf::Vector2f pos, int node){
            
            // If node x does not contain a body, put the new body b here.
            if(qtree[node].quad.size < (qtree[node].quad.center.x - pos.x) && qtree[node].quad.size < (qtree[node].quad.center.y - pos.y) && qtree[node].mass == 0){
                
                if(qtree[node].mass == 0){
                    
                    qtree[node].mass = mass;
                    qtree[node].centerOfMass = pos;
                    return;
                }
                // If node x is an internal node, update the center-of-mass and total mass of x. 
                // Recursively insert the body b in the appropriate quadrant.
                else{
                    
                    qtree[node].centerOfMass = (qtree[node].centerOfMass*qtree[node].mass + pos*mass) / (qtree[node].mass + mass);
                    qtree[node].mass += mass;
                }
            }


            // If node x is an external node, say containing a body named c, then there are two bodies b and c in the same region. 
            //Subdivide the region further by creating four children. 
            //Then, recursively insert both b and c into the appropriate quadrant(s). 
            //Since b and c may still end up in the same quadrant, there may be several subdivisions during a single insertion. 
            //Finally, update the center-of-mass and total mass of x.
            if(qtree[node].quad.size < (qtree[node].quad.center - pos).length()){

                subdivide(node);
                insert(mass, pos, node+1);
                insert(qtree[node].mass, qtree[node].centerOfMass, node+1);
                qtree[node].centerOfMass = (qtree[node].centerOfMass*qtree[node].mass + pos*mass) / (qtree[node].mass + mass);
                qtree[node].mass += mass;

            }

        }

    };
    
}


namespace Burnes_Hut{

    using namespace Barnes_Hut_struct;

    void compute_forces(){
        
    }
}

#endif // BARNESHUT_HPP