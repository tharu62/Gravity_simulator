#include <iostream>
#include "SFML/Graphics.hpp"
#include "application.hpp"
#include "main_input_handler.hpp"

int main(int argc, char *argv[]) 
{
    
    switch(handle(argv[1])){
        case -1:
            exit(0);
        default:
            break;
    }

    std::cout << "Running on SFML Version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << std::endl;

    static Application simulator(1280u, 720u, std::atoi(argv[1]), argv[2]);
    simulator.run();
    // simulator.~Application();

    return 0;
}