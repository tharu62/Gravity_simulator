#include <iostream>
#include "SFML/Graphics.hpp"
// #include "celestial_body.hpp"
#include "planet.hpp"
#include "sun.hpp"
#include "black_hole.hpp"

using namespace std;

int main() 
{
    
    Planet earth;
    auto window = sf::RenderWindow(sf::VideoMode({800u, 600u}), "CMake SFML Project");
    window.setFramerateLimit(144);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();
        window.display();
    }
    
    cout << "Hello, World!" << endl;
    cout << "SFML Version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << endl;
    return 0;
}