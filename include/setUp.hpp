#include <iostream>
#include <random>
#include <cmath>
#include <ctime>
#include <map>
#include <unistd.h>
#include "SFML/Graphics.hpp"
#include "vector_operator.hpp"
#include "planet.hpp"
#include "sun.hpp"
#include "black_hole.hpp"


void sort(Celestial_body *galaxy, sf::VertexArray &points)
{

    int *arr = new int[GALAXY_DIMENSION];

    for(int i=0; i<GALAXY_DIMENSION; ++i){

    }

}

/**
 * @brief Initialization of the galaxy with a random numeber of planets and a suns or black holes at the center.
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

    for(int i = 1; i < GALAXY_DIMENSION; ++i){

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

    for(int i = 0; i < GALAXY_DIMENSION; ++i){

        circle[i].setRadius(galaxy[i].radius);
        switch (galaxy[i].radius)
        {
        case 2:
            circle[i].setFillColor(sf::Color(255, 255, 255));
            break;
        case 109:
            circle[i].setFillColor(sf::Color(255, 255, 255));
            break;
        case 5:
            circle[i].setOutlineThickness(0.6);
            circle[i].setOutlineColor(sf::Color(255, 255, 255));
            circle[i].setFillColor(sf::Color(0, 0, 0));
            break;

        default:
            break;
        }
        circle[i].setPosition(galaxy[i].position);
        circle[i].setOrigin({(float) galaxy[i].radius, (float) galaxy[i].radius});

    }
}

/**
 * @brief 
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

    for(int i = 1; i < GALAXY_DIMENSION; ++i){

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
        // temp.velocity = {0.f, 0.f};
        temp.acceleration = {0.f, 0.f};
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

    for(int i = 1; i < GALAXY_DIMENSION; ++i){
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

    for(int i = 0; i < GALAXY_DIMENSION; ++i){
        circle[i].setRadius(galaxy[i].radius);
        switch (galaxy[i].radius)
        {
        case 1:
            circle[i].setFillColor(sf::Color(200, 200, 200));
            break;
        case 109:
            circle[i].setFillColor(sf::Color(150, 150, 200));
            break;
        case 5:
            circle[i].setOutlineThickness(0.6);
            circle[i].setOutlineColor(sf::Color::White);
            break;

        default:
            break;
        }
        circle[i].setPosition(galaxy[i].position);
        circle[i].setOrigin({(float) galaxy[i].radius, (float) galaxy[i].radius});
    }
}
