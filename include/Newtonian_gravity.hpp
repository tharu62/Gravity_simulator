#ifndef NEWTON_HPP
#define NEWTON_HPP

#include <iostream>
#include <cmath>
#include "SFML/GpuPreference.hpp"
#include "celestial_body.hpp"

#define G 1

namespace Newton{

    /**
     * @brief Computes the Gravitational forces between each celestial body to update the acceleration of each celestial body.
     */
    void compute_forces(Celestial_body *galaxy, sf::CircleShape *circle){

        // double r_2;                 // Distance squared between celestial bodies
        float magnitude;            // Magnitude of the direction vector 
        float magnitude_sq;         // Magnitude squared 
        sf::Vector2f direction;     // Direction vector between two celestial bodies   
    
        for(int i=0; i < GALAXY_DIMENSION; ++i){

            galaxy[i].acceleration = {0.f, 0.f};

            for(int j=0; j < GALAXY_DIMENSION; ++j){
    
                if(i != j){
                    
                    // Alternative method to calculate the acceleration:
                    // r_2 = (galaxy[i].position.x - galaxy[j].position.x)*(galaxy[i].position.x - galaxy[j].position.x) + (galaxy[i].position.y - galaxy[j].position.y)*(galaxy[i].position.y - galaxy[j].position.y);
                    // if(r_2 >= (galaxy[i].radius+galaxy[j].radius)*(galaxy[i].radius+galaxy[j].radius)){
                    //     direction = galaxy[j].position - galaxy[i].position;
                    //     direction /= (float) sqrt(direction.x * direction.x + direction.y * direction.y);
                    //     galaxy[i].acceleration += direction * G*(galaxy[j].mass/r_2);
                    // }

                    direction = galaxy[j].position - galaxy[i].position;
                    magnitude_sq = direction.x*direction.x + direction.y*direction.y;
                    if(magnitude_sq >= (galaxy[i].radius+galaxy[j].radius)*(galaxy[i].radius+galaxy[j].radius)){
                        magnitude = sqrt(magnitude_sq);
                        
                        galaxy[i].acceleration += direction * G * (galaxy[j].mass/(magnitude_sq * magnitude));
                    }
                }
            }
        }
    }
}


#endif // NEWTON_HPP