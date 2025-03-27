#ifndef NEWTON_HPP
#define NEWTON_HPP

#include <iostream>
#include <cmath>
#include "SFML/GpuPreference.hpp"
#include "celestial_body.hpp"

#define CORRECTION_FACTOR 1


namespace Newton{

    /**
     * @brief Computes the Gravitational forces between each celestial body to update the acceleration of each celestial body.
     */
    void compute_forces(Celestial_body *galaxy, sf::CircleShape *circle){

        double F;           // Gravitational Force as scalar
        double r_2;         // Square of the distance between celestial bodies
        float magnitude;
        sf::Vector2f direction;
    
        for(int i=0; i < GALAXY_DIMENSION; ++i){
            for(int j=0; j < GALAXY_DIMENSION; ++j){
    
                r_2 = (galaxy[i].position.x - galaxy[j].position.x)*(galaxy[i].position.x - galaxy[j].position.x) + (galaxy[i].position.y - galaxy[j].position.y)*(galaxy[i].position.y - galaxy[j].position.y);

                if(r_2 != 0.0 && sqrt(r_2) >= (galaxy[i].radius+galaxy[j].radius)){
                    F = CORRECTION_FACTOR*(galaxy[i].mass + galaxy[j].mass) / r_2;
                    direction = galaxy[j].position - galaxy[i].position;
                    magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
                    if (magnitude != 0) {
                        direction /= magnitude; // Normalize the vector
                    }
                    galaxy[i].acceleration.x += direction.x * (F/galaxy[i].mass);
                    galaxy[i].acceleration.y += direction.y * (F/galaxy[i].mass);
                }
                // std::cout << "dir " << direction.x << " " << direction.y << std::endl;
                
            }
        }
    
    }

}


#endif // NEWTON_HPP