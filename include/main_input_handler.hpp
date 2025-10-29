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
        std::cout << "      s  = sun centered galaxy             (Exact Newton gravity acceleration update)" << std::endl;
        std::cout << "      b  = black hole centered galaxy      (Exact Newton gravity acceleration update)" << std::endl;
        std::cout << "      r  = pure random body simulation     (Exact Newton gravity acceleration update)" << std::endl;
        std::cout << "      so = Solar System with CircleShape   (Exact Newton gravity acceleration update)" << std::endl;
        std::cout << std::endl;
        std::cout << "      **RECOMENDED FOR HIGH NUMBER OF BODIES** " << std::endl;
        std::cout << "      ss = S with ArrayVertex of Points instead of CircleShape for bodies (Barnes-Hut algorithm for acceleration update) " << std::endl;
        std::cout << "      bb = B with ArrayVertex of Points instead of CircleShape for bodies (Barnes-Hut algorithm for acceleration update) " << std::endl;
        std::cout << "      rr = R with ArrayVertex of Points instead of CircleShape for bodies (Barnes-Hut algorithm for acceleration update) " << std::endl;
        std::cout << std::endl;

        exit(0);
    }

    if((std::string) input[2] != "s" && (std::string) input[2] != "b" && (std::string) input[2] != "r" && (std::string) input[2] != "so"){
        if((std::string) input[2] != "ss" && (std::string) input[2] != "bb" && (std::string) input[2] != "rr"){
        
            std::cout << "error: invalid argument." << std::endl;
            std::cout << "Use the command 'help' to check all possible arguments" << std::endl;
            exit(0);
        }
    }

    return;
}