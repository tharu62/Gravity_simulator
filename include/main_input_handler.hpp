#include <iostream>
#include <string>

void handle(int size, char* input[])
{
    
    if(size == 1 || atoi(input[1]) < 0){
        
        std::cerr << "\033[0;31m" << "error: invalid argument." << std::endl;
        std::cerr << "Use the command 'help' to check all possible correct arguments" << std::endl;
        exit(0);

    }

    if((std::string)input[1] == "help"){
        
        std::cout << std::endl;
        std::cout << "  cmd syntax : simulator.exe <arg1> <arg2> " << std::endl;
        std::cout << "  <arg1> = number of bodies to simulate. " << std::endl;
        std::cout << "  <arg2> = type of simulation : " << std::endl;
        std::cout << std::endl;
        std::cout << "      S  = sun centered galaxy             (Exact Newton gravity acceleration update)" << std::endl;
        std::cout << "      B  = black hole centered galaxy      (Exact Newton gravity acceleration update)" << std::endl;
        std::cout << "      R  = pure random body simulation     (Exact Newton gravity acceleration update)" << std::endl;
        std::cout << "      SO = Solar System with CircleShape   (Exact Newton gravity acceleration update)" << std::endl;
        std::cout << std::endl;
        std::cout << "      **RECOMENDED FOR HIGH NUMBER OF BODIES** " << std::endl;
        std::cout << "      SS = S with ArrayVertex of Points instead of CircleShape for bodies (Barnes-Hut algorithm for acceleration update) " << std::endl;
        std::cout << "      BB = B with ArrayVertex of Points instead of CircleShape for bodies (Barnes-Hut algorithm for acceleration update) " << std::endl;
        std::cout << "      RR = R with ArrayVertex of Points instead of CircleShape for bodies (Barnes-Hut algorithm for acceleration update) " << std::endl;
        std::cout << std::endl;

        exit(0);
    }

    // if((std::string) input[2] != "s" && (std::string) input[2] != "B" && (std::string) input[2] != "R"){
    //     if((std::string) input[2] != "Ps" && (std::string) input[2] != "PB" && (std::string) input[2] != "PR"){
        
    //         std::cout << "error: invalid parameter value." << std::endl;
    //         std::cout << "Use the command <help> to check all possible correct parameter values" << std::endl;
    //         exit(0);
    //     }
    // }

    return;
}