#ifndef SETUP_HPP
#define SETUP_HPP

#include <iostream>
#include <random>
#include <cmath>
#include <ctime>
#include <map>
#include <unistd.h>
#include "SFML/Graphics.hpp"
#include "vector_operator.hpp"
#include "celestial_body.hpp"

float G  = 6.6743e-11;
// float G = 1;
float AU = 149597870700;
float RADIUS_SCALE = 250000/AU;
float DISTANCE_SCALE = 2500/AU;

extern int GALAXY_DIMENSION;

/**
 * @brief Initialization of the galaxy with a random numeber of planets and a sun or a black hole at the center (Celestial bodies as CircleShape).
 */
void setUp(Celestial_body *galaxy, sf::CircleShape *circle)
{

    std::random_device rd;
    std::mt19937 eng(rd());
    std::normal_distribution<double> distribution1{640.0, 640.0};
    std::normal_distribution<double> distribution2(360.0, 360.0);

    float rand_1;
    float rand_2;
    sf::Vector2f direction;

    // Black hole in the center of the screen
        // Black_hole bh = Black_hole();
        // bh.set_mass();
        // bh.radius = 5;
        // bh.position = {620, 360};
        // bh.prev_position = bh.position;
        // bh.velocity = {0.f, 0.f};
        // bh.acceleration = {0.f, 0.f};
        // galaxy[0] = bh;

    // Sun in the center of the screen
        Sun s = Sun();
        s.set_mass();
        s.radius = 109;
        s.position = {640, 360};
        s.prev_position = s.position;
        s.velocity = {0.f, 0.f};
        s.acceleration = {0.f, 0.f};
        galaxy[0] = s;

    for(u_int32_t i = 1; i < GALAXY_DIMENSION; ++i){

        rand_1 = distribution1(eng);
        rand_2 = distribution2(eng);

        Planet temp = Planet();
        temp.set_mass();
        temp.radius = 2;
        temp.position = {rand_1, rand_2};
        temp.prev_position = temp.position;
        // direction = sf::Vector2f({640, 360}) - temp.position;
        // std::ignore = direction.rotatedBy(sf::degrees(180));
        // direction /= (float) sqrt(direction.x*direction.x + direction.y*direction.y); 
        // direction *=  (float)sqrt(galaxy[0].mass/(sf::Vector2f({640, 360}) - temp.position).length());
        // temp.velocity = {-direction.y, direction.x};
        temp.velocity = {0.f, 0.f};
        temp.acceleration = {0.f, 0.f};
        galaxy[i] = temp;

    }

    for(u_int32_t i = 0; i < GALAXY_DIMENSION; ++i){

        circle[i].setRadius(galaxy[i].radius);
        if(galaxy[i].radius == 2){
            circle[i].setFillColor(sf::Color(255, 255, 255));
        }    
        if(galaxy[i].radius == 109){
            circle[i].setFillColor(sf::Color(255, 255, 255));
        }
        if(galaxy[i].radius == 5){
            circle[i].setOutlineThickness(0.6);
            circle[i].setOutlineColor(sf::Color(255, 255, 255));
            circle[i].setFillColor(sf::Color(0, 0, 0));
        }
        circle[i].setPosition(galaxy[i].position);
        circle[i].setOrigin({(float) galaxy[i].radius, (float) galaxy[i].radius});

    }
}

/**
 * @brief Initialization of the galaxy with a random numeber of planets and a sun or a black hole at the center (Celestial bodies as Points).
 */
void setUp(Celestial_body *galaxy, sf::VertexArray &points)
{

    std::random_device rd;
    std::mt19937 eng(rd());
    std::normal_distribution<double> distribution1{640.0, 640.0};
    std::normal_distribution<double> distribution2(360.0, 360.0);

    float rand_1;
    float rand_2;
    sf::Vector2f direction;

    // Black hole in the center of the screen
        Black_hole bh = Black_hole();
        bh.set_mass();
        bh.radius = 5;
        bh.position = {640, 360};
        bh.prev_position = bh.position;
        bh.velocity = {0.f, 0.f};
        bh.acceleration = {0.f, 0.f};
        galaxy[0] = bh;
        points[0].position = bh.position;
        points[0].color = sf::Color(255, 0, 0);

    // Sun in the center of the screen
        // Sun s = Sun();
        // s.set_mass();
        // s.radius = 109;
        // s.position = {620, 360};
        // s.prev_position = s.position;
        // s.velocity = {0.f, 0.f};
        // s.acceleration = {0.f, 0.f};
        // galaxy[0] = s;
        // points[0].position = s.position;
        // points[0].color = sf::Color(255, 0, 0);

    for(u_int32_t i = 1; i < GALAXY_DIMENSION; ++i){

        rand_1 = distribution1(eng);
        rand_2 = distribution2(eng);

        Planet temp = Planet();
        temp.set_mass();
        temp.radius = 2;
        temp.position = {rand_1, rand_2};
        temp.prev_position = temp.position;
        direction = sf::Vector2f({640, 360}) - temp.position;
        std::ignore = direction.rotatedBy(sf::degrees(180));
        direction /= (float) sqrt(direction.x*direction.x + direction.y*direction.y);
        direction *=  (float) sqrt(galaxy[0].mass/(sf::Vector2f({640, 360}) - temp.position).length());
        temp.velocity = {-direction.y, direction.x};
        temp.acceleration = temp.velocity * (sf::Vector2f({640, 360}) - temp.position).lengthSquared();
        galaxy[i] = temp;
        points[i].position = temp.position;
        points[i].color = sf::Color(255, 255, 255);
    }

}

/**
 * @brief Variant of function setUp with all celestial bodies in random positions.
 */
void setUp_rand(Celestial_body *galaxy, sf::CircleShape *circle)
{

    float rand_1;
    float rand_2;
    srand(time(0));

    for(u_int32_t i = 1; i < GALAXY_DIMENSION; ++i){
        int seed = rand()%100;
        rand_1 = rand()%1280;
        rand_2 = rand()%720;

        if(seed == 0 || seed > 2){
            Planet temp = Planet();
            temp.set_mass();
            temp.radius = 2;
            temp.position = {rand_1, rand_2};
            temp.prev_position = temp.position;
            temp.velocity = {0, 0};
            temp.acceleration = {0, 0};
            galaxy[i] = temp;
        }
        if(seed == 1){
            Sun temp = Sun();
            temp.set_mass();
            temp.radius = 109;
            temp.position = {rand_1, rand_2};
            temp.prev_position = temp.position;
            temp.velocity = {0, 0};
            temp.acceleration = {0, 0};
            galaxy[i] = temp;
        }
        if(seed == 2){
            Black_hole temp = Black_hole();
            temp.set_mass();
            temp.radius = 5;
            temp.position = {rand_1, rand_2};
            temp.prev_position = temp.position;
            temp.velocity = {0, 0};
            temp.acceleration = {0, 0};
            galaxy[i] = temp;
        }
    }

    for(u_int32_t i = 0; i < GALAXY_DIMENSION; ++i){
        circle[i].setRadius(galaxy[i].radius);
        if(galaxy[i].radius == 2){
            circle[i].setFillColor(sf::Color(255, 255, 255));
        }    
        if(galaxy[i].radius == 109){
            circle[i].setFillColor(sf::Color(255, 255, 255));
        }
        if(galaxy[i].radius == 5){
            circle[i].setOutlineThickness(0.6);
            circle[i].setOutlineColor(sf::Color(255, 255, 255));
            circle[i].setFillColor(sf::Color(0, 0, 0));
        }
        circle[i].setPosition(galaxy[i].position);
        circle[i].setOrigin({(float) galaxy[i].radius, (float) galaxy[i].radius});
    }
}

/**
 * @brief Variant of function setUp for setting up the Solar System with the Sun at the center and the 8 main planets in orbit.
 */
void set_up_Solar_System(Celestial_body *galaxy, sf::CircleShape *circle){

    sf::Vector2f direction;

    // Sun in the center of the screen
    galaxy[0] = Planet();
    galaxy[0].mass = 1.989e30;
    galaxy[0].radius = 696340000;
    galaxy[0].position = {640, 360};
    galaxy[0].prev_position = galaxy[1].position;
    galaxy[0].velocity = {0, 0};
    galaxy[0].acceleration = {0.f, 0.f};
    circle[0].setRadius(galaxy[0].radius * RADIUS_SCALE);
    circle[0].setFillColor(sf::Color(255, 255, 255));
    circle[0].setPosition(galaxy[0].position);
    circle[0].setOrigin({galaxy[0].radius * RADIUS_SCALE, galaxy[0].radius * RADIUS_SCALE});

    // Mercury
    galaxy[1] = Planet();
    galaxy[1].mass = 3.285e23;
    galaxy[1].radius = 2439700;
    galaxy[1].position = {640.f + 0.4f * (float) AU, 360.f};
    galaxy[1].prev_position = galaxy[1].position;
    direction = sf::Vector2f({640, 360}) - galaxy[1].position;
    std::ignore = direction.rotatedBy(sf::degrees(180));
    direction /= (float) sqrt(direction.x*direction.x + direction.y*direction.y);
    direction *=  (float) sqrt(G * galaxy[0].mass / (0.4 * AU));
    galaxy[1].velocity = {-direction.y, direction.x};
    // temp.velocity = {0.f, 0.f};
    galaxy[1].acceleration = {0.f, 0.f};
    circle[1].setRadius(galaxy[1].radius * RADIUS_SCALE);
    circle[1].setFillColor(sf::Color(183, 184, 185));    
    circle[1].setPosition({galaxy[1].position.x * DISTANCE_SCALE, galaxy[1].position.y});
    circle[1].setOrigin({galaxy[1].radius * RADIUS_SCALE, galaxy[1].radius * RADIUS_SCALE});

    // Venus
    galaxy[2] = Planet();
    galaxy[2].mass = 4.867e24;
    galaxy[2].radius = 6052000;
    galaxy[2].position = {640.f + 0.72f * (float) AU, 360};
    galaxy[2].prev_position = galaxy[2].position;
    direction = sf::Vector2f({640, 360}) - galaxy[2].position;
    std::ignore = direction.rotatedBy(sf::degrees(180));
    direction /= (float) sqrt(direction.x*direction.x + direction.y*direction.y);
    direction *=  (float) sqrt(G * galaxy[0].mass/(0.72 * AU));
    galaxy[2].velocity = {-direction.y, direction.x};
    // temp.velocity = {0.f, 0.f};
    galaxy[2].acceleration = {0.f, 0.f};
    circle[2].setRadius(galaxy[2].radius * RADIUS_SCALE);
    circle[2].setFillColor(sf::Color(238,203,139));
    circle[2].setPosition({galaxy[2].position.x * DISTANCE_SCALE, galaxy[2].position.y});
    circle[2].setOrigin({galaxy[2].radius * RADIUS_SCALE, galaxy[2].radius * RADIUS_SCALE});

    // Earth
    galaxy[3] = Planet();
    galaxy[3].mass = 5.972e24;
    galaxy[3].radius = 6378000;
    galaxy[3].position = {640 + AU, 360};
    galaxy[3].prev_position = galaxy[3].position;
    direction = sf::Vector2f({640, 360}) - galaxy[3].position;
    std::ignore = direction.rotatedBy(sf::degrees(180));
    direction /= (float) sqrt(direction.x*direction.x + direction.y*direction.y);
    direction *=  (float) sqrt(G * galaxy[0].mass/(AU));
    galaxy[3].velocity = {-direction.y, direction.x};
    // temp.velocity = {0.f, 0.f};
    galaxy[3].acceleration = {0.f, 0.f};
    circle[3].setRadius(galaxy[3].radius * RADIUS_SCALE);
    circle[3].setFillColor(sf::Color(79,76,176));
    circle[3].setPosition({galaxy[3].position.x * DISTANCE_SCALE, galaxy[3].position.y});
    circle[3].setOrigin({galaxy[3].radius * RADIUS_SCALE, galaxy[3].radius * RADIUS_SCALE});

    // Mars
    galaxy[4] = Planet();
    galaxy[4].mass = 6.39e23;
    galaxy[4].radius = 3389500;
    galaxy[4].position = {640.f + 1.52f * (float) AU, 360};
    galaxy[4].prev_position = galaxy[4].position;
    direction = sf::Vector2f({640, 360}) - galaxy[4].position;
    std::ignore = direction.rotatedBy(sf::degrees(180));
    direction /= (float) sqrt(direction.x*direction.x + direction.y*direction.y);
    direction *=  (float) sqrt(G * galaxy[0].mass/(1.52 * AU));
    galaxy[4].velocity = {-direction.y, direction.x};
    // temp.velocity = {0.f, 0.f};
    galaxy[4].acceleration = {0.f, 0.f};
    circle[4].setRadius(galaxy[4].radius * RADIUS_SCALE);
    circle[4].setFillColor(sf::Color(193,68,14));
    circle[4].setPosition({galaxy[4].position.x * DISTANCE_SCALE, galaxy[4].position.y});
    circle[4].setOrigin({galaxy[4].radius * RADIUS_SCALE, galaxy[4].radius * RADIUS_SCALE});

    // Jupiter
    galaxy[5] = Planet();
    galaxy[5].mass = 1.89813e27;
    galaxy[5].radius = 69911000;
    galaxy[5].position = {640.f + 5.2f * (float) AU, 360};
    galaxy[5].prev_position = galaxy[5].position;
    direction = sf::Vector2f({640, 360}) - galaxy[5].position;
    std::ignore = direction.rotatedBy(sf::degrees(180));
    direction /= (float) sqrt(direction.x*direction.x + direction.y*direction.y);
    direction *=  (float) sqrt(G * galaxy[0].mass/(5.2 * AU));
    galaxy[5].velocity = {-direction.y, direction.x};
    // temp.velocity = {0.f, 0.f};
    galaxy[5].acceleration = {0.f, 0.f};
    circle[5].setRadius(galaxy[5].radius * RADIUS_SCALE);
    circle[5].setFillColor(sf::Color(201,144,57));
    circle[5].setPosition({galaxy[5].position.x * DISTANCE_SCALE, galaxy[5].position.y});
    circle[5].setOrigin({galaxy[5].radius * RADIUS_SCALE, galaxy[5].radius * RADIUS_SCALE});   

    // Saturn
    galaxy[6] = Planet();
    galaxy[6].mass = 5.683e26;
    galaxy[6].radius = 60268000;
    galaxy[6].position = {640.f + 9.5f * (float) AU, 360};
    galaxy[6].prev_position = galaxy[6].position;
    direction = sf::Vector2f({640, 360}) - galaxy[6].position;
    std::ignore = direction.rotatedBy(sf::degrees(180));
    direction /= (float) sqrt(direction.x*direction.x + direction.y*direction.y);
    direction *=  (float) sqrt(G * galaxy[0].mass/(9.5 * AU));
    galaxy[6].velocity = {-direction.y, direction.x};
    // temp.velocity = {0.f, 0.f};
    galaxy[6].acceleration = {0.f, 0.f};
    circle[6].setRadius(galaxy[6].radius * RADIUS_SCALE);
    circle[6].setFillColor(sf::Color(206,184,184));
    circle[6].setPosition({galaxy[6].position.x * DISTANCE_SCALE, galaxy[6].position.y});
    circle[6].setOrigin({galaxy[6].radius * RADIUS_SCALE, galaxy[6].radius * RADIUS_SCALE});

    // Uranus
    galaxy[7] = Planet();
    galaxy[7].mass = 8,681e25;
    galaxy[7].radius = 25362000;
    galaxy[7].position = {640.f + 19.2f * (float) AU, 360};
    galaxy[7].prev_position = galaxy[7].position;
    direction = sf::Vector2f({640, 360}) - galaxy[7].position;
    std::ignore = direction.rotatedBy(sf::degrees(180));
    direction /= (float) sqrt(direction.x*direction.x + direction.y*direction.y);
    direction *=  (float) sqrt(G * galaxy[0].mass/(19.2 * AU));
    galaxy[7].velocity = {-direction.y, direction.x};
    // temp.velocity = {0.f, 0.f};
    galaxy[7].acceleration = {0.f, 0.f};
    circle[7].setRadius(galaxy[7].radius * RADIUS_SCALE);
    circle[7].setFillColor(sf::Color(172, 229, 238));
    circle[7].setPosition({galaxy[7].position.x * DISTANCE_SCALE, galaxy[7].position.y});
    circle[7].setOrigin({galaxy[7].radius * RADIUS_SCALE, galaxy[7].radius * RADIUS_SCALE});

    // Neptune
    galaxy[8] = Planet();
    galaxy[8].mass = 1,024e26;
    galaxy[8].radius = 24622000;
    galaxy[8].position = {640.f + 30.1f * (float) AU, 360};
    galaxy[8].prev_position = galaxy[8].position;
    direction = sf::Vector2f({640, 360}) - galaxy[8].position;
    std::ignore = direction.rotatedBy(sf::degrees(180));
    direction /= (float) sqrt(direction.x*direction.x + direction.y*direction.y);
    direction *=  (float) sqrt(G * galaxy[0].mass/(30.1 * AU));
    galaxy[8].velocity = {-direction.y, direction.x};
    // temp.velocity = {0.f, 0.f};
    galaxy[8].acceleration = {0.f, 0.f};
    circle[8].setRadius(galaxy[8].radius * RADIUS_SCALE);
    circle[8].setFillColor(sf::Color(91,93,223));
    circle[8].setPosition({galaxy[8].position.x * DISTANCE_SCALE, galaxy[8].position.y});
    circle[8].setOrigin({galaxy[8].radius * RADIUS_SCALE, galaxy[8].radius * RADIUS_SCALE});

}

#endif // SETUP_HPP