#include <iostream>
#include <random>
#include <cmath>
#include <ctime>
#include <unistd.h>
#include "SFML/Graphics.hpp"
#include "application.hpp"

int main() 
{
    Application simulator(1066u, 600u);
    simulator.run();
    
    std::cout << "Window closed!" << std::endl;
    std::cout << "SFML Version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << std::endl;
    return 0;
}