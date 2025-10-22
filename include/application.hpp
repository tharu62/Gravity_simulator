#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <iostream>
#include <ctime>

#include "SFML/Graphics.hpp"
#include "vector_operator.hpp"
#include "eventHandler.hpp"
#include "setUp.hpp"
#include "position_integration.hpp"
#include "Newtonian_gravity.hpp"
#include "Barnes_Hut_algorithm.hpp"
#include "collision&merge.hpp"
#include "sort_celestial_body.hpp"

int GALAXY_DIMENSION;


class Application
{

    public:
    unsigned int width;
    unsigned int height;
    
    private:

    char setter;
    bool moving = false;
    bool paused = false;
    sf::Vector2f oldPos;

    Celestial_body *galaxy;
    sf::CircleShape *circle;
    sf::VertexArray points;
    Barnes_Hut_struct::Quadtree *q;

    public:
    
    /**
     * @todo add setter functionality for different initial galaxy setups.
     */
    Application(unsigned int x, unsigned int y, int nob, char *set): width(x), height(y), setter(*set){
        GALAXY_DIMENSION = nob;
    }

    /**
     * @brief Main constructor for Application class.
     */
    Application(unsigned int x, unsigned int y, int nob): width(x), height(y){
        GALAXY_DIMENSION = nob;
        std::cout << "Simulator constructed!" << std::endl;
    }

    /**
     * @brief Deconstructor for Application class.
     */
    ~Application(){ 
        delete[] galaxy;
        delete[] circle;
        delete q;
        std::cout << "Simulator deconstracted!" << std::endl; 
    }

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

    /**
     * @brief Main loop for simulation of celestial bodies and graphical output on window.
     */
    void run(){
        std::cout << "Simulator running!" << std::endl;
        
        auto window = sf::RenderWindow(sf::VideoMode(sf::Vector2u{width, height}), "Gravity Simulator");
        sf::View view(sf::FloatRect({0.f, 0.f}, {(float)width, (float)height}));
        sf::View view2(sf::FloatRect({0.f, 0.f}, {40.f, 30.f}));
        view2.setViewport(sf::FloatRect({0.f, 0.f}, {0.04f, 0.04f}));
        window.setVerticalSyncEnabled(true);

        sf::Font font;
        std::ignore = font.openFromFile("/home/deshan/Documents/Code/C++/Gravity_simulator/include/arial_narrow_7/arial_narrow_7.ttf");
        sf::Text framerate(font);
        framerate.setString("0");
        framerate.setCharacterSize(20);
        framerate.setFillColor(sf::Color::Red);
        framerate.setStyle(sf::Text::Bold);
        framerate.setPosition({0.f, 0.f});

        galaxy = new Celestial_body[GALAXY_DIMENSION];
        circle = new sf::CircleShape[GALAXY_DIMENSION];
        points = sf::VertexArray{sf::PrimitiveType::Points, (std::size_t) GALAXY_DIMENSION};
        
        setUp(galaxy, circle, points, "Points", "solar_system");
        q = new Barnes_Hut_struct::Quadtree();
        
        clock_t start = 0;
        clock_t end = 0;
        while (window.isOpen())
        {

            start = clock();

            while (const std::optional event = window.pollEvent())
            {
                EventHandler(event, view, window, oldPos, moving, paused);
            }
            
            // Code to handle simulation and drawing on window. You can add your own code here. 
            // (use Pause key = p to pause or resume the simulation)
            if(!paused){

                // Collision detection, merge and sort methods
                    // collision_detecion(galaxy);
                    // merge(galaxy);                                       // no implementation yet
                    // sort(galaxy, points);
                    // sort_all(galaxy, points);
                
                // Acceleration update methods
                    // Newton::compute_forces(galaxy);
                    Newton::compute_forces_solar_system(galaxy);
                    // Burnes_Hut::compute_forces(galaxy, *q);
    
                // Position update methods (CircleShape)
                    // Verlet::update_position(galaxy, circle);
                    // Euler::update_position(galaxy, circle);
                    Euler::update_position_solar_system(galaxy, circle);
                    // Runge_Kutta::update_position(galaxy, circle);        // no implementation yet
                
                // Position update methods (Points)
                    // Verlet::update_position(galaxy, points);
                    // Euler::update_position(galaxy, points);
                    // Runge_Kutta::update_position(galaxy, points);        // no implementation yet

            }

            // draw after clearing the window
            window.clear();

            window.setView(view2);
            window.draw(framerate);
            
            window.setView(view);
            // window.draw(points);
            for(int i = 0; i < GALAXY_DIMENSION; ++i){
                window.draw(circle[i]);
            }

            // for(int i=0; i<q->qtree.size(); ++i){
            //     draw_box(window, q->qtree[i].center, q->qtree[i].size);
            // }
            // sf::VertexArray point(sf::PrimitiveType::Points, 1);
            // point[0].position = q->qtree[0].centerOfMass;
            // point[0].color = sf::Color::Green;
            // window.draw(point);
            
            // double elapsed = double(end - start)/CLOCKS_PER_SEC;
            // std::cout << elapsed << std::endl;
                    
            end = clock();
            framerate.setString(std::to_string((int) (CLOCKS_PER_SEC / double(end - start))));

            window.display();
                
        }

        std::cout << "Simulator stopped!" << std::endl;
    }

};

#endif // APPLICATION_HPP