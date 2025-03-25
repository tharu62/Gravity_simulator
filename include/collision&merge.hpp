#ifndef COLLISION_MERGE_HPP
#define COLLISION_MERGE_HPP

#include <vector>
#include <iostream>
#include <cmath>
#include "celestial_body.hpp"
#include "planet.hpp"
#include "sun.hpp"
#include "black_hole.hpp"

/**
 * @brief Check if two celestial bodies have collided
 */
bool collided(Celestial_body &a, Celestial_body &b)
{
    // double distance = sqrt(pow(a.get_position()[0] - b.get_position()[0], 2) + pow(a.get_position()[1] - b.get_position()[1], 2));
    // if(distance < a.get_radius() + b.get_radius()){
    //     return true;
    // }
    return false;
}

/**
 * @brief Merge two celestial bodies
 */
Celestial_body merge(Celestial_body &a, Celestial_body &b, type t)
{ 
    Celestial_body c;

    switch (t){
    case planet:
        c = Planet();
        break;
    case sun:
        c = Sun();
        break;  
    case black_hole:
        c = Black_hole();
        break;
    default:
        Celestial_body c;
        break;
    }

    c.set_mass(a.get_mass() + b.get_mass());
    // c.set_radius(); boh
    // c.set_color(); boh

    // c.set_position({(a.get_position()[0] + b.get_position()[0])/2, (a.get_position()[1] + b.get_position()[1])/2});
    // c.set_velocity(); boh
    // c.set_acceleration(); boh
    return c;

}

#endif // COLLISION_MERGE_HPP