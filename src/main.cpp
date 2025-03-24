#include <iostream>
#include "SFML/Graphics.hpp"
// #include "celestial_body.hpp"
#include "planet.hpp"
#include "sun.hpp"
#include "black_hole.hpp"

using namespace std;

int main() 
{
    auto window = sf::RenderWindow(sf::VideoMode({800u, 600u}), "CMake SFML Project");
    window.setFramerateLimit(144);
    
    sf::CircleShape circle;
    circle.setRadius(1);
    circle.setOutlineColor(sf::Color::Red);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition({10, 20});

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // code to handle simulation and drawing on window

            
        }

        // draw after clearing the window
        window.clear();
        window.draw(circle);
        window.display();
    }
    
    cout << "Window closed!" << endl;
    cout << "SFML Version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << endl;
    return 0;
}