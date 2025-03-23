#include <iostream>
#include "SFML/Graphics.hpp"

using namespace std;

int main() 
{
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
    return 0;
}