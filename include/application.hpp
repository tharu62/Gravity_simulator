#ifndef APPLICATION_HPP
#define APPLICATION_HPP

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
#include "eventHandler.hpp"
#include "position_integration.hpp"

int GALAXY_DIMENSION;

#include "Newtonian_gravity.hpp"
#include "Barnes_Hut_algorithm.hpp"

class Application{

    public:
    unsigned int width;
    unsigned int height;
    char set;

    private:

    /**
     * @brief Initialization of the galaxy with a random numeber of planets and a suns or black holes at the center.
     */
    void setUp(Celestial_body *galaxy, sf::CircleShape *circle){

        std::random_device rd;
        std::mt19937 eng(rd());
      
        std::normal_distribution<double> distribution1{640.0, 640.0};
        std::normal_distribution<double> distribution2(360.0, 360.0);

        float rand_1;
        float rand_2;

        // Black hole in the center of the screen
        Black_hole temp = Black_hole();
        temp.set_mass();
        temp.set_radius(5);
        temp.set_position({620, 360});
        temp.prev_position = temp.position;
        temp.set_acceleration({0, 0});
        galaxy[0] = temp;

        // Sun in the center of the screen
        // Sun temp = Sun();
        // temp.set_mass();
        // temp.set_radius(109);
        // temp.set_position({620, 360});
        // temp.prev_position = temp.position;
        // temp.set_acceleration({0, 0});
        // galaxy[0] = temp;

        for(int i = 1; i < GALAXY_DIMENSION; ++i){
            // int seed = rand()%100;
            // rand_1 = rand()%1280;
            rand_1 = distribution1(eng);
            // rand_2 = rand()%720;
            rand_2 = distribution2(eng);

            Planet temp = Planet();
            temp.set_mass();
            temp.set_radius(2);
            temp.set_position({rand_1, rand_2});
            temp.prev_position = temp.position;
            temp.set_acceleration({0.0, 0.0});
            galaxy[i] = temp;
        }

        for(int i = 0; i < GALAXY_DIMENSION; ++i){
            circle[i].setRadius(galaxy[i].radius);
            switch (galaxy[i].radius)
            {
            case 2:
                circle[i].setFillColor(sf::Color(200, 200, 200));
                break;
            case 109:
                circle[i].setFillColor(sf::Color(150, 150, 50));
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

    /**
     * @brief Variant of function setUp with all celestial bodies in random positions.
     */
    void setUp_rand(Celestial_body *galaxy, sf::CircleShape *circle){
    
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
                temp.set_radius(2);
                temp.set_position({rand_1, rand_2});
                temp.prev_position = temp.position;
                temp.set_acceleration({0, 0});
                galaxy[i] = temp;
            }
            if(seed == 1){
                Sun temp = Sun();
                temp.set_mass();
                temp.set_radius(109);
                temp.set_position({rand_1, rand_2});
                temp.prev_position = temp.position;
                temp.set_acceleration({0, 0});
                galaxy[i] = temp;
            }
            if(seed == 2){
                Black_hole temp = Black_hole();
                temp.set_mass();
                temp.set_radius(5);
                temp.set_position({rand_1, rand_2});
                temp.prev_position = temp.position;
                temp.set_acceleration({0, 0});
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

    public:

    Application(unsigned int x, unsigned int y, int galaxy_dimention): width(x), height(y){
        // width = x;
        // height = y;
        GALAXY_DIMENSION = galaxy_dimention;
    }

    ~Application(){ std::cout << "Simulator deconstracted!" << std::endl; }

    /**
     * @brief Main loop for simulation of celestial bodies and graphical output on window.
     */
    void run(){
        std::cout << "Simulator open!" << std::endl;

        auto window = sf::RenderWindow(sf::VideoMode({width, height}), "Gravity Simulator");
        sf::View view(sf::FloatRect({0.f, 0.f}, {1280.f, 720.f}));
        window.setFramerateLimit(60);
        
        Celestial_body galaxy[GALAXY_DIMENSION];
        sf::CircleShape circle[GALAXY_DIMENSION];
        
        setUp(galaxy, circle);   

        while (window.isOpen())
        {
            while (const std::optional event = window.pollEvent())
            {
    
                EventHandler(event, view, window);
    
            }
    
            // code to handle simulation and drawing on window
            Newton::compute_forces(galaxy, circle);
            // Burnes_Hut::compute_forces();
    
            for(int i=0; i < GALAXY_DIMENSION; ++i){
                Verlet::update_position(galaxy[i], circle[i]);
            }
    
            // draw after clearing the window
            window.clear();
            for(int i = 0; i < GALAXY_DIMENSION; ++i){
                window.setView(view);
                // sf::RectangleShape line({10.f, 5.f});
                // line.setOrigin(galaxy[i].position);  
                // line.rotate(sf::degrees(galaxy[i].acceleration.angle().asDegrees()));
                // line.setFillColor(sf::Color::Green);
                // window.draw(line);
                window.draw(circle[i]);
            }
            window.display();
        }
        
        std::cout << "Simulator closed!" << std::endl;
    }

};

#endif // APPLICATION_HPP