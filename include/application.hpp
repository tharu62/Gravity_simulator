#include <iostream>
#include <random>
#include <cmath>
#include <ctime>
#include <unistd.h>
#include "SFML/Graphics.hpp"
#include "vector_operator.hpp"
#include "planet.hpp"
#include "sun.hpp"
#include "black_hole.hpp"
#include "eventHandler.hpp"

#define GALAXY_DIMENSION 3

class Application{

    public:
    unsigned int width;
    unsigned int height;






    private:

    /**
     * @brief Initialization of the galaxy with a random numeber of planets, suns and black holes.
     */
    void setUp(Celestial_body *galaxy, sf::CircleShape *circle){
    
        float rand_1;
        float rand_2;
        srand(time(0));

        for(int i = 0; i < GALAXY_DIMENSION; ++i){
            int seed = rand()%100;
            rand_1 = rand()%1060;
            rand_2 = rand()%600;

            if(seed == 0 || seed > 4){
                Planet temp = Planet();
                temp.set_mass();
                temp.set_radius(1);
                temp.set_position({rand_1, rand_2});
                temp.prev_position = temp.position;
                rand_1 = rand()%30 - 15.0;
                rand_2 = rand()%30 -15.0;
                temp.set_acceleration({rand_1, rand_2});
                temp.set_color();
                galaxy[i] = temp;
            }
            if(seed == 1 || seed == 2 || seed == 3){
                Sun temp = Sun();
                temp.set_mass();
                temp.set_radius(rand()%2 +5);
                temp.set_position({rand_1, rand_2});
                temp.prev_position = temp.position;
                rand_1 = rand()%10 - 5.0;
                rand_2 = rand()%10 - 5.0;
                temp.set_acceleration({rand_1, rand_2});
                temp.set_color();
                galaxy[i] = temp;
            }
            if(seed == 4){
                Black_hole temp = Black_hole();
                temp.set_mass();
                temp.set_radius(rand()%4 +10);
                temp.set_position({rand_1, rand_2});
                temp.prev_position = temp.position;
                rand_1 = rand()%5 - 2.0;
                rand_2 = rand()%5 - 2.0;
                temp.set_acceleration({rand_1, rand_2});
                temp.set_color();
                galaxy[i] = temp;
            }
        }

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
     * @brief Updates the position of any kind of celestial body using Verlet Integration (without velocities).
     */
    void update_position(Celestial_body &body, sf::CircleShape &circle){

        float time_delta = 0.001;
        sf::Vector2f temp = body.position;

        body.position.x = (temp.x * 2) - body.prev_position.x + (body.acceleration.x * pow(time_delta, 2));
        body.position.y = (temp.y * 2) - body.prev_position.y + (body.acceleration.y * pow(time_delta, 2));

        body.prev_position = temp;

        circle.setPosition(body.position);
    }

    /**
     * @brief Computes the Gravitational forces between each celestial body to update the acceleration of each celestial body.
     */
    void compute_forces(Celestial_body *galaxy, sf::CircleShape *circle){

        double F;           // Gravitational Force as scalar
        double r_2;         // Square of the distance between celestial bodies
        float magnitude;
        sf::Vector2f direction;
    
        for(int i=0; i < GALAXY_DIMENSION; ++i){
            for(int j=0; j < GALAXY_DIMENSION; ++j){
    
                r_2 = (pow(galaxy[i].position.x/1000 - galaxy[j].position.x/1000, 2) + pow(galaxy[i].position.y/1000 - galaxy[j].position.y/1000, 2));
                if(r_2 != 0){
    
                    F = (galaxy[i].mass + galaxy[j].mass) / r_2;
                    direction = galaxy[j].position - galaxy[i].position;
                    magnitude = sqrt(direction.x * direction.x + direction.y * direction.y);
                    if (magnitude != 0) {
                        direction /= magnitude; // Normalize the vector
                    }
                    
                    galaxy[i].acceleration.x += direction.x * (F/galaxy[i].mass);
                    galaxy[i].acceleration.y += direction.y * (F/galaxy[i].mass);
    
                }
                
            }
        }
    
    }






    public:
    Application(unsigned int x, unsigned int y){
        width = x;
        height = y;
    }

    void run(){
        auto window = sf::RenderWindow(sf::VideoMode({width, height}), "Gravity Simulator");
        sf::View view(sf::FloatRect({0.f, 0.f}, {1000.f, 600.f}));
        window.setView(view);
        window.setFramerateLimit(60);
        
        Celestial_body galaxy[GALAXY_DIMENSION];
        sf::CircleShape circle[GALAXY_DIMENSION];
        
        setUp(galaxy, circle);
    
        while (window.isOpen())
        {
            while (const std::optional event = window.pollEvent())
            {
    
                EventHandler(event);
    
                if (event->is<sf::Event::Closed>())
                {
                    window.close();
                }
    
            }
    
            // code to handle simulation and drawing on window
            compute_forces(galaxy, circle);
    
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
    }

};

