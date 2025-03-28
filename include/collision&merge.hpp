#ifndef COLLISION_MERGE_HPP
#define COLLISION_MERGE_HPP

#include <vector>
#include <iostream>
#include <cmath>
#include "celestial_body.hpp"

extern int GALAXY_DIMENSION;

/**
 * @brief Check if two celestial bodies have collided and corrects velocity in moudele and direction
 */
void collision_detecion(Celestial_body *galaxy)
{
    float distance_sq;
    sf::Vector2f vel_dif;
    sf::Vector2f dir;

    for(int i = 0; i < GALAXY_DIMENSION; ++i){
        for(int j = 0; j < GALAXY_DIMENSION; ++j){
            if(i != j){
                distance_sq = (galaxy[i].position.x - galaxy[j].position.x)*(galaxy[i].position.x - galaxy[j].position.x) + (galaxy[i].position.y - galaxy[j].position.y)*(galaxy[i].position.y - galaxy[j].position.y);
                
                if(distance_sq < (galaxy[i].radius + galaxy[j].radius)*(galaxy[i].radius + galaxy[j].radius)){

                    vel_dif = (galaxy[i].velocity - galaxy[j].velocity);
                    dir = (galaxy[i].position - galaxy[j].position);
                    if(dir.length() != 0){
                        galaxy[i].velocity = galaxy[i].velocity - ((2*galaxy[j].mass)/(galaxy[i].mass+galaxy[j].mass)) * (vel_dif.dot(dir) / dir.lengthSquared()) * dir;
                    }
                }
            }
        }
    }
}

/**
 * @brief Merge two celestial bodies upon collision
 */
Celestial_body merge(Celestial_body &a, Celestial_body &b, type t)
{ 
    Celestial_body c;
    // c.set_radius(); boh
    // c.set_color(); boh

    // c.set_position({(a.get_position()[0] + b.get_position()[0])/2, (a.get_position()[1] + b.get_position()[1])/2});
    // c.set_velocity(); boh
    // c.set_acceleration(); boh
    return c;

}

#endif // COLLISION_MERGE_HPP