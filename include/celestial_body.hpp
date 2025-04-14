#ifndef CELESTIAL_BODY_HPP
#define CELESTIAL_BODY_HPP

#include <vector>
#include "SFML/Graphics.hpp"

struct Celestial_body
{   
    double mass;
    float radius;
    sf::Vector2f position;
    sf::Vector2f prev_position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
};

#endif // CELESTIAL_BODY_HPP