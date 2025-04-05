#ifndef SUN_HPP
#define SUN_HPP

#include <vector>
#include <random>
#include <iostream>
#include "celestial_body.hpp"

struct Sun : public Celestial_body{

    void set_mass(){
        mass = 33000000;
    }

};

#endif // SUN_HPP