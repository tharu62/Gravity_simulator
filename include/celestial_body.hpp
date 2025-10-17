#ifndef CELESTIAL_BODY_HPP
#define CELESTIAL_BODY_HPP

#include <vector>
#include "SFML/Graphics.hpp"

/**
 * @brief Celestial body struct that contains the mass, radius, position, previous position, velocity and acceleration of a celestial body.
 * @details total size of struct = 4*2 + 8*4 = 40 bytes.
 */
struct Celestial_body
{   
    float mass;                 // 4 bytes
    float radius;               // 4 bytes
    sf::Vector2f position;      // 8 bytes
    sf::Vector2f prev_position; // 8 bytes
    sf::Vector2f velocity;      // 8 bytes
    sf::Vector2f acceleration;  // 8 bytes
};

struct Black_hole : public Celestial_body{

    // int event_horizon;

    void set_mass(){
        mass = 1265400000;
    }   

    // void set_event_horizon(double r){
    //     // the event horizon is the point of no return
    //     // the radius of the black hole should be smaller than the event horizon
    //     event_horizon = r*2;
    // }

};

struct Planet : public Celestial_body{

    void set_mass(){
        mass = 1000;
    }

};

struct Sun : public Celestial_body{

    void set_mass(){
        mass = 33000000;
    }

};

#endif // CELESTIAL_BODY_HPP