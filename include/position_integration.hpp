#ifndef POSINTEGRATION_HPP
#define POSINTEGRATION_HPP

#include <iostream>
#include <cmath>
#include "SFML/GpuPreference.hpp"
#include "celestial_body.hpp"
#include "vector_operator.hpp"

extern int GALAXY_DIMENSION;

namespace Verlet{
    
    sf::Vector2f temp;
    const float dt = 0.01;
    
    /**
     * @brief Updates the position of any kind of celestial body using Verlet Integration (without velocities).
     */
    void update_position(Celestial_body *body, sf::CircleShape *circle){
        
        for(u_int32_t i = 0; i < GALAXY_DIMENSION; ++i){
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

        for(u_int32_t i = 0; i < GALAXY_DIMENSION; ++i){
            temp = body[i].position;

            body[i].position.x = (temp.x * 2) - body[i].prev_position.x + (body[i].acceleration.x * (dt * dt));
            body[i].position.y = (temp.y * 2) - body[i].prev_position.y + (body[i].acceleration.y * (dt * dt));

            body[i].prev_position = temp;

            points[i].position = body[i].position;
        }
        
    }

}

namespace Euler{

    const float dt = 0.001;
   
    
    /**
     * @brief Updates the position of any kind of celestial body using Euler's Method with velocities.
     */
    void update_position(Celestial_body *body, sf::CircleShape *circle){

        for(u_int32_t i = 0; i < GALAXY_DIMENSION; ++i){
            body[i].velocity += body[i].acceleration * dt;
            body[i].position += body[i].velocity * dt;

            circle[i].setPosition(body[i].position);
        }
    }

    /**
     * @brief Updates the position of any kind of celestial body using Euler's Method with velocities.
     */
    void update_position(Celestial_body *body, sf::VertexArray &points){
 

        for(u_int32_t i = 0; i < GALAXY_DIMENSION; ++i){
            
            body[i].velocity += body[i].acceleration * dt;
            body[i].position += body[i].velocity * dt;

            points[i].position = body[i].position;
        }
        

    }


    float AU = 149597870700;
    float RADIUS_SCALE = 250000/AU;
    float DISTANCE_SCALE = 2500/AU;
    /**
     * @brief Updates the position of any kind of celestial body using Euler's Method with velocities for the Solar System.
     * @note This method is specifically designed for simulating the Solar System, where distances and sizes are scaled down for better visualization.
     * @note The method uses different scaling factors for distances and sizes to ensure that celestial bodies are visible and proportionate on the screen.
     */
    void update_position_solar_system(Celestial_body *body, sf::CircleShape *circle){

        for(u_int32_t i = 0; i < GALAXY_DIMENSION; ++i){
            body[i].velocity += body[i].acceleration * dt;
            body[i].position += body[i].velocity * dt;

            circle[i].setPosition(circle[i].getPosition() + body[i].velocity * dt * DISTANCE_SCALE);
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