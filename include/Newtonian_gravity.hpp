#ifndef NEWTON_HPP
#define NEWTON_HPP

#include <iostream>
#include <cmath>
#include "SFML/GpuPreference.hpp"
#include "celestial_body.hpp"

extern int GALAXY_DIMENSION;
#define G1 1.f

namespace Newton{

    /**
     * @brief Computes the Gravitational forces between each celestial body to update the acceleration of each celestial body.
     */
    void compute_forces(Celestial_body *galaxy){

        float magnitude;            // Magnitude of the direction vector 
        float magnitude_sq;         // Magnitude squared 
        sf::Vector2f direction;     // Direction vector between two celestial bodies   
    
        for(int i=0; i < GALAXY_DIMENSION; ++i){

            galaxy[i].acceleration = {0.f, 0.f};

            for(int j=0; j < GALAXY_DIMENSION; ++j){
    
                if(i != j){

                    direction = galaxy[j].position - galaxy[i].position;
                    magnitude_sq = (direction.x*direction.x + direction.y*direction.y);
                    if(magnitude_sq >= 0.1f){
                        magnitude = sqrt(magnitude_sq);
                        galaxy[i].acceleration += direction * G1 * (galaxy[j].mass/(magnitude_sq * magnitude));
                    }

                }
            }
        }
    }

    /**
     * @brief Computes the Gravitational forces between each celestial body to update the acceleration of each celestial body in the solar_system.
     */
    void compute_forces_solar_system(Celestial_body *galaxy){

        float magnitude;            // Magnitude of the direction vector 
        float magnitude_sq;         // Magnitude squared 
        sf::Vector2f direction;     // Direction vector between two celestial bodies   
    
        for(int i=0; i < GALAXY_DIMENSION; ++i){

            galaxy[i].acceleration = {0.f, 0.f};

            for(int j=0; j < GALAXY_DIMENSION; ++j){
    
                if(i != j){

                    direction = galaxy[j].position - galaxy[i].position;
                    magnitude_sq = (direction.x*direction.x + direction.y*direction.y);
                    if(magnitude_sq >= 0.001f){
                        magnitude = sqrt(magnitude_sq);
                        galaxy[i].acceleration += direction * G * (galaxy[j].mass/(magnitude_sq * magnitude));
                    }

                }
            }
        }
    }
}


#endif // NEWTON_HPP