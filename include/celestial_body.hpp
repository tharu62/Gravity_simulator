#ifndef CELESTIAL_BODY_HPP
#define CELESTIAL_BODY_HPP

#include <vector>
#include "SFML/Graphics.hpp"

enum type{
    planet,
    sun,
    black_hole
};

class Celestial_body
{   
    public:
    type type;
    double mass;
    double radius;
    sf::Color color;
    sf::Vector2f position;
    sf::Vector2f prev_position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;

    private:

    public:

    Celestial_body(){
        mass = 0;
        radius = 0;
    }

    void set_mass(double m){
        mass = m;
    }
    
    void set_radius(double r){
        radius = r;
    }

    void set_color(int r, int g, int b){
        color = {r, g, b};
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