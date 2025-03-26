#ifndef SUN_HPP
#define SUN_HPP

#include <vector>
#include <random>
#include <iostream>
#include "celestial_body.hpp"

class Sun : public Celestial_body{

    private:

    public:
    Sun(){
        type = sun;
    }

    void set_color(){
        // the sun should have a bright color like yellow, orange, red ecc...
        Celestial_body::set_color(255, 255, 0);
    }

    void set_mass(){
        Celestial_body::set_mass(333000);
    }

};

#endif // SUN_HPP