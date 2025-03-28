#ifndef POSINTEGRATION_HPP
#define POSINTEGRATION_HPP

#include <iostream>
#include <cmath>
#include "SFML/GpuPreference.hpp"
#include "celestial_body.hpp"
#include "vector_operator.hpp"

#define MAX_VELOCITY 47000

namespace Verlet{
    
    sf::Vector2f temp;
    float dt = 0.01;
    
    /**
     * @brief Updates the position of any kind of celestial body using Verlet Integration (without velocities).
     */
    void update_position(Celestial_body &body, sf::CircleShape &circle){
        
        temp = body.position;

        body.position.x = (temp.x * 2) - body.prev_position.x + (body.acceleration.x * (dt * dt));
        body.position.y = (temp.y * 2) - body.prev_position.y + (body.acceleration.y * (dt * dt));

        body.prev_position = temp;

        circle.setPosition(body.position);
    }

}

namespace Euler{

    float dt = 0.01;
    
    /**
     * @brief Updates the position of any kind of celestial body using Euler's Method with velocities.
     */
    void update_position(Celestial_body &body, sf::CircleShape &circle){

        body.velocity += body.acceleration * dt;
        // if(body.velocity.length() > MAX_VELOCITY){
        //     body.velocity /= 1.5f;
        // }
        body.position += body.velocity * dt;

        circle.setPosition(body.position);
    }

}


#endif // POSINTEGRATION_HPP