#ifndef CELESTIAL_BODY_HPP
#define CELESTIAL_BODY_HPP

#include <vector>
#include "SFML/Graphics.hpp"

class Celestial_body
{   
    public:
    int mass;
    int radius;
    sf::Vector2f position;
    sf::Vector2f prev_position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    public:

    void set_mass(float m){
        mass = m;
    }
    
    void set_radius(int r){
        radius = r;
    }

    void set_position(sf::Vector2f p){
        position = p;
    }

    void set_velocity(sf::Vector2f v){
        velocity = v;
    }

    void set_acceleration(sf::Vector2f a){
        acceleration = a;
    }

};

#endif // CELESTIAL_BODY_HPP