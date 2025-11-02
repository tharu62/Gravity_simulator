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
        std::cout << "      sc  = sun centered galaxy with CircleShape          (Exact Newtonian gravity acceleration update)" << std::endl;
        std::cout << "      bc  = black hole centered galaxy with CircleShape   (Exact Newtonian gravity acceleration update)" << std::endl;
        std::cout << "      rc  = pure random body simulation with CircleShape  (Exact Newtonian gravity acceleration update)" << std::endl;
        std::cout << "      ss = Solar System with CircleShape (only)           (Exact Newtonian gravity acceleration update)" << std::endl;
        std::cout << std::endl;
        std::cout << "      **RECOMENDED FOR HIGH NUMBER OF BODIES** " << std::endl;
        std::cout << "      sp = sun centered galaxy with ArrayVertex of Points             (Barnes-Hut algorithm for acceleration update) " << std::endl;
        std::cout << "      bp = black hole centered galaxy with ArrayVertex of Points      (Barnes-Hut algorithm for acceleration update) " << std::endl;
        std::cout << "      rp = pure random body simulation with ArrayVertex of Points     (Barnes-Hut algorithm for acceleration update) " << std::endl;
        std::cout << std::endl;

        exit(0);
    }

    if((std::string) input[2] != "sc" && (std::string) input[2] != "bc" && (std::string) input[2] != "rc" && (std::string) input[2] != "ss"){
        if((std::string) input[2] != "sp" && (std::string) input[2] != "bp" && (std::string) input[2] != "rp"){
        
            std::cerr << "\033[0;31m" << "error: invalid argument." << std::endl;
            std::cerr << "Use the command 'help' to check all possible correct arguments" << std::endl;
            exit(0);
        }
    }

    return;
}