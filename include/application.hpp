#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <iostream>
#include <random>
#include <cmath>
#include <ctime>
#include <map>
#include <unistd.h>
#include <thread>
#include <chrono>
#include "SFML/Graphics.hpp"
#include "vector_operator.hpp"
#include "planet.hpp"
#include "sun.hpp"
#include "black_hole.hpp"
#include "eventHandler.hpp"

int GALAXY_DIMENSION;

#include "setUp.hpp"
#include "position_integration.hpp"
#include "Newtonian_gravity.hpp"
#include "Barnes_Hut_algorithm.hpp"
#include "collision&merge.hpp"

class Application
{

    public:
    unsigned int width;
    unsigned int height;
    
    private:
    // unused var
    char setter;
    // <mooving> and <oldPos> are used for view movement
    bool moving = false;
    sf::Vector2f oldPos;

    public:

    /**
     * @brief Debug function that draws the bounding boxes of the quadtree produced by the Barnes-Hut method for updating acceleration.
     */
    void draw_box(sf::RenderWindow &window, sf::Vector2f pos, float size){
        sf::RectangleShape box;
        box.setSize({size*2, size*2});
        box.setOrigin({size, size});
        box.setPosition(pos);
        box.setOutlineColor(sf::Color::Red);
        box.setOutlineThickness(1.f);
        box.setFillColor(sf::Color::Transparent);
        window.draw(box);
    }

    Application(unsigned int x, unsigned int y, int galaxy_dimention, char *set): width(x), height(y){
        // width = x;
        // height = y;
        GALAXY_DIMENSION = galaxy_dimention;
        setter = *set;
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

        Celestial_body *galaxy = new Celestial_body[GALAXY_DIMENSION];
        // sf::CircleShape *circle = new sf::CircleShape[GALAXY_DIMENSION];
        sf::VertexArray points{sf::PrimitiveType::Points, (std::size_t) GALAXY_DIMENSION};
        
        // setUp(galaxy, circle);
        setUp(galaxy, points);
        
        Barnes_Hut_struct::Quadtree *q = new Barnes_Hut_struct::Quadtree();

        while (window.isOpen())
        {
            while (const std::optional event = window.pollEvent())
            {
                EventHandler(event, view, window, oldPos, moving);
            }

            // Code to handle simulation and drawing on window
            
            // Collision detection and merge methods
                // collision_detecion(galaxy);
                // merge(galaxy);
                // std::thread t1(collision_detecion, galaxy);
                // std::thread t2(merge, galaxy);
            
            // Acceleration update methods
                // Newton::compute_forces(galaxy);
                Burnes_Hut::compute_forces(galaxy, *q);
                // std::thread t3(Newton::compute_forces, galaxy);
                // std::thread t4([galaxy, q](){ Burnes_Hut::compute_forces(galaxy, *q); });

            // t1.join();
            // t2.join();
            // t3.join();
            // t4.join();


            // Position update methods (CircleShape)
                // Verlet::update_position(galaxy, circle);
                // Euler::update_position(galaxy, circle);
                // Runge_Kutta::update_position(galaxy, circle);
            
            // Position update methods (Points)
                // Verlet::update_position(galaxy, points);
                Euler::update_position(galaxy, points);
                // Runge_Kutta::update_position(galaxy, points);

            // draw after clearing the window
            window.clear();
            window.setView(view);
            // for(int i = 0; i < GALAXY_DIMENSION; ++i){
                //     window.draw(circle[i]);
                // }
                // for(int i=0; i<q->qtree.size(); ++i){
                //     draw_box(window, q->qtree[i].center, q->qtree[i].size);
                // }
                window.draw(points);
                // sf::VertexArray point(sf::PrimitiveType::Points, 1);
                // point[0].position = q->qtree[0].centerOfMass;
                // point[0].color = sf::Color::Green;
                // window.draw(point);
                
            window.display();
                // clock_t start = clock();
                // clock_t end = clock();
                // double elapsed = double(end - start)/CLOCKS_PER_SEC;
                // std::cout << elapsed << std::endl;
        }

        delete[] galaxy;
        delete q;
        
        std::cout << "Simulator closed!" << std::endl;
    }

};

#endif // APPLICATION_HPP