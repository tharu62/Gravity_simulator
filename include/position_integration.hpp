#ifndef POSINTEGRATION_HPP
#define POSINTEGRATION_HPP

#include <iostream>
#include <cmath>
#include "SFML/GpuPreference.hpp"
#include "celestial_body.hpp"
namespace Verlet{
    
sf::Vector2f temp;

    /**
     * @brief Updates the position of any kind of celestial body using Verlet Integration (without velocities).
     */
    void update_position(Celestial_body &body, sf::CircleShape &circle){
        
        float time_delta = 0.01;
        temp = body.position;

        body.position.x = (temp.x * 2) - body.prev_position.x + (body.acceleration.x * pow(time_delta, 2));
        body.position.y = (temp.y * 2) - body.prev_position.y + (body.acceleration.y * pow(time_delta, 2));

        body.prev_position = temp;

        circle.setPosition(body.position);
    }

}

#endif // POSINTEGRATION_HPP