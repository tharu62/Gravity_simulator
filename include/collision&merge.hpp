#ifndef COLLISION_MERGE_HPP
#define COLLISION_MERGE_HPP

#include <vector>
#include <iostream>
#include <cmath>
#include "celestial_body.hpp"

extern int GALAXY_DIMENSION;
#define DETECTION_MARGIN 30
#define RESISTANCE_FACTOR 0.99

extern int GALAXY_DIMENSION;

/**
 * @brief Check if two celestial bodies have collided and corrects velocity in module and direction
 */
void collision_detecion(Celestial_body *galaxy)
{
    float distance_sq;
    float distance;
    sf::Vector2f vel_dir;
    sf::Vector2f dir;
    float penetration;

    for(int i = 0; i < GALAXY_DIMENSION; ++i){
        
        for(int j = 0; j < GALAXY_DIMENSION; ++j){

            if(i != j ){

                distance_sq = (galaxy[i].position.x - galaxy[j].position.x)*(galaxy[i].position.x - galaxy[j].position.x) + (galaxy[i].position.y - galaxy[j].position.y)*(galaxy[i].position.y - galaxy[j].position.y);
                
                if(distance_sq < (galaxy[i].radius + galaxy[j].radius)*(galaxy[i].radius + galaxy[j].radius)){
                    
                    vel_dir = (galaxy[i].velocity - galaxy[j].velocity);
                    dir = (galaxy[i].position - galaxy[j].position);
                    if(dir.length() != 0){
                        galaxy[i].velocity = (galaxy[i].velocity - dir * ((2*galaxy[j].mass)/(galaxy[i].mass+galaxy[j].mass)) * (vel_dir.dot(dir) / dir.lengthSquared())) * RESISTANCE_FACTOR;
                    }
                    
                    // This makes sure the bodies don't go inside each other (at least not often).
                    if(galaxy[i].mass < galaxy[j].mass){

                        distance = sqrt(distance_sq);
                        penetration = ((galaxy[i].radius + galaxy[j].radius) - distance);
                        galaxy[i].position += penetration * ((galaxy[i].position - galaxy[j].position) / distance);

                    }
                }

            }

        }
    }
}

/**
 * @brief Merge two celestial bodies upon collision
 */
void merge(Celestial_body *galaxy)
{ 


}

#endif // COLLISION_MERGE_HPP