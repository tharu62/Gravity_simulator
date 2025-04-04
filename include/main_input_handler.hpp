#include <iostream>

int handle(std::string input1){

    if(input1 == "help"){

        std::cout << std::endl;
        std::cout << "  cmd syntax : simulator.exe <arg1> <arg2> " << std::endl;
        std::cout << "  <arg1> = number of bodies to simulate " << std::endl;
        std::cout << "  <arg2> = type of simulation : " << std::endl;
        std::cout << "      S = sun centered on galaxy " << std::endl;
        std::cout << "      B = balck hole centered on galaxy " << std::endl;
        std::cout << "      R = pure random body simulation " << std::endl;
        std::cout << std::endl;
        std::cout << "      **RECOMENDED FOR HIGH NUMBER OF BODIES** " << std::endl;
        std::cout << "      PS = S with ArrayVertex of Points instead of CircleShape for bodies (improves performance drastically) " << std::endl;
        std::cout << "      PB = as PS " << std::endl;
        std::cout << "      PR = as PS " << std::endl;
        std::cout << std::endl;

        return -1;
    }

    if(input1 == "" || input1 == " "){

        return 0;
    }

    return 0;
}