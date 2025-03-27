#include <iostream>
#include "SFML/Graphics.hpp"
#include "application.hpp"


int main(int argc, char *argv[]) 
{
    std::cout << "Running on SFML Version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << std::endl;

    static Application simulator(1280u, 720u, std::atoi(argv[1]));
    simulator.run();
    // simulator.~Application();

    return 0;
}