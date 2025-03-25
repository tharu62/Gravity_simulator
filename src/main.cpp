#include <iostream>
#include <random>
#include <cmath>
#include "SFML/Graphics.hpp"
#include "vector_operator.hpp"
#include "planet.hpp"
#include "sun.hpp"
#include "black_hole.hpp"


#define GALAXY_DIMENSION 10

using namespace std;

/**
 * @brief Initialization of the galaxy with a random numeber of planets, suns and black holes.
 */
void setUp(Celestial_body *galaxy, sf::CircleShape *circle){

    for(int i = 0; i < GALAXY_DIMENSION; ++i){
        int seed = rand()%100;
        if(seed == 0 || seed > 4){
            Planet temp = Planet();
            temp.set_mass();
            temp.set_radius(rand()%1 +2);
            temp.set_position({rand()%1060, rand()%600});
            temp.prev_position = temp.position;
            temp.set_acceleration({rand()%30, rand()%30});
            temp.set_color();
            galaxy[i] = temp;
        }
        if(seed == 1 || seed == 2 || seed == 3){
            Sun temp = Sun();
            temp.set_mass();
            temp.set_radius(rand()%2 +5);
            temp.set_position({rand()%1060, rand()%600});
            temp.prev_position = temp.position;
            temp.set_acceleration({rand()%10, rand()%10});
            temp.set_color();
            galaxy[i] = temp;
        }
        if(seed == 4){
            Black_hole temp = Black_hole();
            temp.set_mass();
            temp.set_radius(rand()%4 +10);
            temp.set_position({rand()%1060, rand()%600});
            temp.prev_position = temp.position;
            temp.set_acceleration({rand()%5, rand()%5});
            temp.set_color();
            galaxy[i] = temp;
        }
    }

    // sorting the galaxy from smallest to the biggest planet, sun, black hole 
    // for(int i = 0; i < GALAXY_DIMENSION; ++i){

    // }
    
    for(int i = 0; i < GALAXY_DIMENSION; ++i){
        circle[i].setRadius(galaxy[i].radius);
        if(galaxy[i].type == black_hole){
            circle[i].setOutlineThickness(0.6);
            circle[i].setOutlineColor(sf::Color::White);
        }else{
            circle[i].setOutlineColor(galaxy[i].color);
        } 
        circle[i].setFillColor(galaxy[i].color);
        circle[i].setPosition(galaxy[i].position);
    }
}


/**
 * @brief Updates the position of any kind of celestial body using Verlet Integration (without velocities)
 */
void update_position(Celestial_body &body, sf::CircleShape &circle){

    float time_delta = 0.01;
    sf::Vector2f temp = body.position;

    body.position.x = (temp.x * 2) - body.prev_position.x + (body.acceleration.x * pow(time_delta, 2));
    body.position.y = (temp.y * 2) - body.prev_position.y + (body.acceleration.y * pow(time_delta, 2));

    body.prev_position = temp;

    circle.setPosition(body.position);
}



int main() 
{
    auto window = sf::RenderWindow(sf::VideoMode({1066u, 600u}), "CMake SFML Project");
    window.setFramerateLimit(60);
    
    Celestial_body galaxy[GALAXY_DIMENSION];
    sf::CircleShape circle[GALAXY_DIMENSION];
    
    setUp(galaxy, circle);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

        }

        // code to handle simulation and drawing on window
        for(int i=0; i < GALAXY_DIMENSION; ++i){
            update_position(galaxy[i], circle[i]);
        }
        

        // draw after clearing the window
        window.clear();
        for(int i = 0; i < GALAXY_DIMENSION; ++i){
            window.draw(circle[i]);
        }
        window.display();
    }
    
    cout << "Window closed!" << endl;
    cout << "SFML Version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << endl;
    return 0;
}