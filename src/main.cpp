#include <iostream>
#include "SFML/Graphics.hpp"
#include "application.hpp"
#include "main_input_handler.hpp"

int main(int argc, char *argv[]) 
{
    
    handle(argc, argv);

    std::cout << "Running Simulation on SFML Version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << std::endl;

    static Application simulator(1280u, 720u, std::atoi(argv[1]));
    // static Application simulator(1280u, 720u, std::atoi(argv[1]), argv[2]);
    simulator.run();
    // simulator.~Application();

    return 0;
}