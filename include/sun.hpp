#ifndef SUN_HPP
#define SUN_HPP

#include <vector>
#include <random>
#include <iostream>
#include "celestial_body.hpp"

class Sun : public Celestial_body{

    private:

    public:

    void set_random_color(){
        // the sun should have a bright color like yellow, orange, red ecc...
        set_color(rand() % 255, rand() % 255, 0);
    }

};

#endif // SUN_HPP