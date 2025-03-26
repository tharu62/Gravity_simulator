#include <iostream>
#include "SFML/Graphics.hpp"
#include "application.hpp"

int main() 
{
    std::cout << "Running on SFML Version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << std::endl;
    
    static Application simulator(1280u, 720u);
    simulator.run();
    // simulator.~Application();

    return 0;
}