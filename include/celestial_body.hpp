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

#endif // CELESTIAL_BODY_HPP