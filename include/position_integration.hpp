#ifndef POSINTEGRATION_HPP
#define POSINTEGRATION_HPP

#include <iostream>
#include <cmath>
#include "SFML/GpuPreference.hpp"
#include "celestial_body.hpp"
#include "vector_operator.hpp"

namespace Verlet{
    
    sf::Vector2f temp;
    float dt = 0.01;
    
    /**
     * @brief Updates the position of any kind of celestial body using Verlet Integration (without velocities).
     */
    void update_position(Celestial_body *body, sf::CircleShape *circle){
        
        for(int i=0; i < GALAXY_DIMENSION; ++i){
            temp = body[i].position;

            body[i].position.x = (temp.x * 2) - body[i].prev_position.x + (body[i].acceleration.x * (dt * dt));
            body[i].position.y = (temp.y * 2) - body[i].prev_position.y + (body[i].acceleration.y * (dt * dt));

            body[i].prev_position = temp;

            circle[i].setPosition(body[i].position);
        }
    }

    /**
     * @brief Updates the position of any kind of celestial body using Verlet Integration (without velocities).
     */
    void update_position(Celestial_body *body, sf::VertexArray &points){

        for(int i=0; i < GALAXY_DIMENSION; ++i){
            temp = body[i].position;

            body[i].position.x = (temp.x * 2) - body[i].prev_position.x + (body[i].acceleration.x * (dt * dt));
            body[i].position.y = (temp.y * 2) - body[i].prev_position.y + (body[i].acceleration.y * (dt * dt));

            body[i].prev_position = temp;

            points[i].position = body[i].position;
        }
        
    }

}

namespace Euler{

    float dt = 0.01;
    
    /**
     * @brief Updates the position of any kind of celestial body using Euler's Method with velocities.
     */
    void update_position(Celestial_body *body, sf::CircleShape *circle){

        for(int i=0; i < GALAXY_DIMENSION; ++i){
            body[i].velocity += body[i].acceleration * dt;
            body[i].position += body[i].velocity * dt;

            circle[i].setPosition(body[i].position);
        }
    }

    /**
     * @brief Updates the position of any kind of celestial body using Euler's Method with velocities.
     */
    void update_position(Celestial_body *body, sf::VertexArray &points){
 

        for(int i=0; i < GALAXY_DIMENSION; ++i){
            body[i].velocity += body[i].acceleration * dt;
            body[i].position += body[i].velocity * dt;

            points[i].position = body[i].position;
        }

    }

}

namespace Runge_Kutta{

    /**
     * @todo
     */
    void update_position(Celestial_body *body, sf::CircleShape *circle){

    }

    /**
     * @todo
     */
    void update_position(Celestial_body *body, sf::VertexArray &circle){

    }
}

#endif // POSINTEGRATION_HPP