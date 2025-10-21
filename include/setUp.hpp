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

float G  = 6.6743e-11f;
float AU = 149597870700.f;

extern int GALAXY_DIMENSION;

/**
 * @brief Initialization of the galaxy with a random numeber of planets and a sun or a black hole at the center (Celestial bodies as CircleShape).
 */
void setUp(Celestial_body *galaxy, sf::CircleShape *circle, std::string type = "default")
{

    std::random_device rd;
    std::mt19937 eng(rd());
    std::normal_distribution<double> distribution1{640.0, 640.0};
    std::normal_distribution<double> distribution2(360.0, 360.0);

    float rand_1;
    float rand_2;
    sf::Vector2f direction;

    // Black hole in the center of the screen
    if(type == "default" || type == "black_hole_centered"){
        Black_hole bh = Black_hole();
        bh.set_mass();
        bh.radius = 5;
        bh.position = {640, 360};
        bh.prev_position = bh.position;
        bh.velocity = {0.f, 0.f};
        bh.acceleration = {0.f, 0.f};
        galaxy[0] = bh;
    }
    // Sun in the center of the screen
    else if(type == "sun_centered"){
        Sun s = Sun();
        s.set_mass();
        s.radius = 109;
        s.position = {640, 360};
        s.prev_position = s.position;
        s.velocity = {0.f, 0.f};
        s.acceleration = {0.f, 0.f};
        galaxy[0] = s;
    }

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
        direction *=  (float)sqrt(galaxy[0].mass/(sf::Vector2f({640, 360}) - temp.position).length());
        temp.velocity = {-direction.y, direction.x};
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
void setUp(Celestial_body *galaxy, sf::VertexArray &points, std::string type = "default")
{

    std::random_device rd;
    std::mt19937 eng(rd());
    std::normal_distribution<double> distribution1{640.0, 640.0};
    std::normal_distribution<double> distribution2(360.0, 360.0);

    float rand_1;
    float rand_2;
    sf::Vector2f direction;

    // Black hole in the center of the screen
    if(type == "default" || type == "black_hole_centered"){
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
    }
    // Sun in the center of the screen
    else if(type == "sun_centered"){
        Sun s = Sun();
        s.set_mass();
        s.radius = 109;
        s.position = {620, 360};
        s.prev_position = s.position;
        s.velocity = {0.f, 0.f};
        s.acceleration = {0.f, 0.f};
        galaxy[0] = s;
        points[0].position = s.position;
        points[0].color = sf::Color(255, 0, 0);
    }
    
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
 * @brief Variant of function setUp with all celestial bodies in random positions.
 */
void setUp_rand(Celestial_body *galaxy, sf::VertexArray &points)
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
        points[i].position = galaxy[i].position;
        if(galaxy[i].radius == 2){
            points[i].color = sf::Color(255, 255, 255);
        }    
        if(galaxy[i].radius == 109){
            points[i].color = sf::Color(0, 0, 255);
        }
        if(galaxy[i].radius == 5){
            points[i].color = sf::Color(255, 0, 0);
        } 
    }
}


sf::Vector2f circle_coordinate(double angle, float radius){
    float rad = angle * static_cast<float>(M_PI) / 180.0f;
    float x = radius * std::cos(angle);
    float y = - radius * std::sin(angle);
    // std::cout << "x: " << x << " y: " << y << std::endl;
    return sf::Vector2f(x, y);
}

/**
 * @brief Variant of function setUp for setting up the Solar System with the Sun at the center and the 8 main planets in orbit.
 */
void setUp_solar_system(Celestial_body *galaxy, sf::CircleShape *circle){

    sf::Vector2f direction;

    // Sun in the center of the screen
    galaxy[0] = Planet();
    galaxy[0].mass = 1.989e30;
    galaxy[0].radius = 696340000;
    galaxy[0].position = {0.f, 0.f};
    galaxy[0].prev_position = galaxy[1].position;
    galaxy[0].velocity = {0, 0};
    galaxy[0].acceleration = {0.f, 0.f};
    circle[0].setRadius((float)log(galaxy[0].radius)/15);
    circle[0].setFillColor(sf::Color(255, 255, 255));
    circle[0].setPosition(log(galaxy[0].position));
    circle[0].setOrigin({(float)log(galaxy[0].radius)/15, (float)log(galaxy[0].radius)/15});

    // Mercury
    galaxy[1] = Planet();
    galaxy[1].mass = 3.285e23;
    galaxy[1].radius = 2439700;
    // galaxy[1].position = {-0.4f * AU, 0.f};
    galaxy[1].position = circle_coordinate(40.0, (0.4f * AU));
    galaxy[1].prev_position = galaxy[1].position;
    direction = galaxy[0].position - galaxy[1].position;
    std::ignore = direction.rotatedBy(sf::degrees(180));
    direction /= (float) sqrt(direction.x*direction.x + direction.y*direction.y);
    direction *=  (float) sqrt(G * galaxy[0].mass / (0.4 * AU));
    galaxy[1].velocity = sf::Vector2f{-direction.y, direction.x};
    // galaxy[1].velocity = sf::Vector2f{-direction.y, direction.x} * 47870.f;
    galaxy[1].acceleration = galaxy[1].velocity * (galaxy[0].position - galaxy[1].position).lengthSquared();
    circle[1].setRadius((float)log(galaxy[1].radius)/20);
    circle[1].setFillColor(sf::Color(183, 184, 185));    
    circle[1].setPosition(log(galaxy[1].position));
    circle[1].setOrigin({(float)log(galaxy[1].radius)/20, (float)log(galaxy[1].radius)/20});

    // Venus
    galaxy[2] = Planet();
    galaxy[2].mass = 4.867e24;
    galaxy[2].radius = 6052000;
    // galaxy[2].position = {0.72f * (float) AU, 0.f};
    galaxy[2].position = circle_coordinate(80.0, (0.72f * AU));
    galaxy[2].prev_position = galaxy[2].position;
    direction = galaxy[0].position - galaxy[2].position;
    std::ignore = direction.rotatedBy(sf::degrees(180));
    direction /= (float) sqrt(direction.x*direction.x + direction.y*direction.y);
    direction *=  (float) sqrt(G * galaxy[0].mass/(0.72 * AU));
    galaxy[2].velocity = sf::Vector2f{-direction.y, direction.x};
    // galaxy[2].velocity = sf::Vector2f{-direction.y, direction.x} * 35020.f;
    galaxy[2].acceleration = galaxy[2].velocity * (galaxy[0].position - galaxy[2].position).lengthSquared();
    circle[2].setRadius((float)log(galaxy[2].radius)/20);
    circle[2].setFillColor(sf::Color(238,203,139));
    circle[2].setPosition(log(galaxy[2].position));
    circle[2].setOrigin({(float)log(galaxy[2].radius)/20, (float)log(galaxy[2].radius)/20});

    // Earth
    galaxy[3] = Planet();
    galaxy[3].mass = 5.972e24;
    galaxy[3].radius = 6378000;
    // galaxy[3].position = {-AU, 0.f};
    galaxy[3].position = circle_coordinate(120.0, (1.f * AU));
    galaxy[3].prev_position = galaxy[3].position;
    direction = galaxy[0].position - galaxy[3].position;
    std::ignore = direction.rotatedBy(sf::degrees(180));
    direction /= (float) sqrt(direction.x*direction.x + direction.y*direction.y);
    direction *=  (float) sqrt(G * galaxy[0].mass/(AU));
    galaxy[3].velocity = sf::Vector2f{-direction.y, direction.x};
    // galaxy[3].velocity = sf::Vector2f{-direction.y, direction.x} * 29780.f;
    galaxy[3].acceleration = galaxy[3].velocity * (galaxy[0].position - galaxy[3].position).lengthSquared();
    circle[3].setRadius((float)log(galaxy[3].radius)/20);
    circle[3].setFillColor(sf::Color(79,76,176));
    circle[3].setPosition(log(galaxy[3].position));
    circle[3].setOrigin({(float)log(galaxy[3].radius)/20, (float)log(galaxy[3].radius)/20});

    // Mars
    galaxy[4] = Planet();
    galaxy[4].mass = 6.39e23;
    galaxy[4].radius = 3389500;
    // galaxy[4].position = {1.52f * (float) AU, 0.f};
    galaxy[4].position = circle_coordinate(160.0, (1.52f * AU));
    galaxy[4].prev_position = galaxy[4].position;
    direction = galaxy[0].position - galaxy[4].position;
    std::ignore = direction.rotatedBy(sf::degrees(180));
    direction /= (float) sqrt(direction.x*direction.x + direction.y*direction.y);
    direction *=  (float) sqrt(G * galaxy[0].mass/(1.52 * AU));
    galaxy[4].velocity = sf::Vector2f{-direction.y, direction.x};
    // galaxy[4].velocity = sf::Vector2f{-direction.y, direction.x} * 24070.f;
    galaxy[4].acceleration = galaxy[4].velocity * (galaxy[0].position - galaxy[4].position).lengthSquared();   
    circle[4].setRadius((float)log(galaxy[4].radius)/20);
    circle[4].setFillColor(sf::Color(193,68,14));
    circle[4].setPosition(log(galaxy[4].position));
    circle[4].setOrigin({(float)log(galaxy[4].radius)/20, (float)log(galaxy[4].radius)/20});

    // Jupiter
    galaxy[5] = Planet();
    galaxy[5].mass = 1.89813e27;
    galaxy[5].radius = 69911000;
    // galaxy[5].position = {-5.2f * (float) AU, 0.f};
    galaxy[5].position = circle_coordinate(200.0, (5.2f * AU));
    galaxy[5].prev_position = galaxy[5].position;
    direction = galaxy[0].position - galaxy[5].position;
    std::ignore = direction.rotatedBy(sf::degrees(180));
    direction /= (float) sqrt(direction.x*direction.x + direction.y*direction.y);
    direction *=  (float) sqrt(G * galaxy[0].mass/(5.2 * AU));
    galaxy[5].velocity = sf::Vector2f{-direction.y, direction.x};
    // galaxy[5].velocity = sf::Vector2f{-direction.y, direction.x} * 13070.f;
    galaxy[5].acceleration = galaxy[5].velocity * (galaxy[0].position - galaxy[5].position).lengthSquared();
    circle[5].setRadius((float)log(galaxy[5].radius)/20);
    circle[5].setFillColor(sf::Color(201,144,57));
    circle[5].setPosition(log(galaxy[5].position));
    circle[5].setOrigin({(float)log(galaxy[5].radius)/20, (float)log(galaxy[5].radius)/20});  

    // Saturn
    galaxy[6] = Planet();
    galaxy[6].mass = 5.683e26;
    galaxy[6].radius = 60268000;
    // galaxy[6].position = {9.5f * (float) AU, 0.f};
    galaxy[6].position = circle_coordinate(240.0, (9.5f * AU));
    galaxy[6].prev_position = galaxy[6].position;
    direction = galaxy[0].position - galaxy[6].position;
    std::ignore = direction.rotatedBy(sf::degrees(180));
    direction /= (float) sqrt(direction.x*direction.x + direction.y*direction.y);
    direction *=  (float) sqrt(G * galaxy[0].mass/(9.5 * AU));
    galaxy[6].velocity = sf::Vector2f{-direction.y, direction.x};
    // galaxy[6].velocity = sf::Vector2f{-direction.y, direction.x} * 9680.f;
    galaxy[6].acceleration = galaxy[6].velocity * (galaxy[0].position - galaxy[6].position).lengthSquared();
    galaxy[6].acceleration = {0.f, 0.f};
    circle[6].setRadius((float)log(galaxy[6].radius)/20);
    circle[6].setFillColor(sf::Color(206,184,184));
    circle[6].setPosition(log(galaxy[6].position));
    circle[6].setOrigin({(float)log(galaxy[6].radius)/20, (float)log(galaxy[6].radius)/20});

    // Uranus
    galaxy[7] = Planet();
    galaxy[7].mass = 8.681e25;
    galaxy[7].radius = 25362000;
    // galaxy[7].position = {-19.2f * (float) AU, 0.f};
    galaxy[7].position = circle_coordinate(280.0, (19.2f * AU));
    galaxy[7].prev_position = galaxy[7].position;
    direction = galaxy[0].position - galaxy[7].position;
    std::ignore = direction.rotatedBy(sf::degrees(180));
    direction /= (float) sqrt(direction.x*direction.x + direction.y*direction.y);
    direction *=  (float) sqrt(G * galaxy[0].mass/(19.2 * AU));
    galaxy[7].velocity = sf::Vector2f{-direction.y, direction.x};
    // galaxy[7].velocity = sf::Vector2f{-direction.y, direction.x} * 6800.f;
    galaxy[7].acceleration = galaxy[7].velocity * (galaxy[0].position - galaxy[7].position).lengthSquared();
    circle[7].setRadius((float)log(galaxy[7].radius)/20);
    circle[7].setFillColor(sf::Color(172, 229, 238));
    circle[7].setPosition(log(galaxy[7].position));
    circle[7].setOrigin({(float)log(galaxy[7].radius)/20, (float)log(galaxy[7].radius)/20});

    // Neptune
    galaxy[8] = Planet();
    galaxy[8].mass = 1.024e26;
    galaxy[8].radius = 24622000;
    // galaxy[8].position = {30.1f * (float) AU, 0.f};
    galaxy[8].position = circle_coordinate(320.0, (30.1f * AU));
    galaxy[8].prev_position = galaxy[8].position;
    direction = galaxy[0].position - galaxy[8].position;
    std::ignore = direction.rotatedBy(sf::degrees(180));
    direction /= (float) sqrt(direction.x*direction.x + direction.y*direction.y);
    direction *=  (float) sqrt(G * galaxy[0].mass/(30.1 * AU));
    galaxy[8].velocity = sf::Vector2f{-direction.y, direction.x};
    // galaxy[8].velocity = sf::Vector2f{-direction.y, direction.x} * 5430.f;
    galaxy[8].acceleration = galaxy[8].velocity * (galaxy[0].position - galaxy[8].position).lengthSquared();
    circle[8].setRadius((float)log(galaxy[8].radius)/20);
    circle[8].setFillColor(sf::Color(91,93,223));
    circle[8].setPosition(log(galaxy[8].position));
    circle[8].setOrigin({(float)log(galaxy[8].radius)/20, (float)log(galaxy[8].radius)/20});

}

/**
 * @brief Generalized setup function to choose between different rendering types and celestial body initializations.
 * @param galaxy Pointer to an array of Celestial_body objects representing the galaxy.
 * @param circle Pointer to an array of sf::CircleShape objects for rendering celestial bodies as circles.
 * @param points Reference to an sf::VertexArray for rendering celestial bodies as points.
 * @param render_type String specifying the rendering type ("CircleShape" or "Points").
 * @param planetary_system String specifying the type of planetary system initialization ("random", "solar_system", "sun_centered", "black_hole_centered").
 */
void setUp(Celestial_body *galaxy, sf::CircleShape *circle, sf::VertexArray &points, std::string render_type, std::string planetary_system){

    if(render_type == "CircleShape"){
        if(planetary_system == "random"){
            setUp_rand(galaxy, circle);
        }
        else if(planetary_system == "solar_system"){
            setUp_solar_system(galaxy, circle);
        }
        else if(planetary_system == "sun_centered"){
            setUp(galaxy, circle);
        }
        else if(planetary_system == "black_hole_centered"){
            setUp(galaxy, circle);
        }
    }
    else if(render_type == "Points"){
        if(planetary_system == "random"){
            setUp_rand(galaxy, points);
        }
        else if(planetary_system == "solar_system"){
            // Solar system setup not implemented for Points rendering
            std::cerr << "Solar system setup not implemented for Points rendering." << std::endl;
        }
        else if(planetary_system == "sun_centered"){
            setUp(galaxy, points);
        }
        else if(planetary_system == "black_hole_centered"){
            setUp(galaxy, points);
        }
    }
}


#endif // SETUP_HPP