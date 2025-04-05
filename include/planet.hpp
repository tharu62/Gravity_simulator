#ifndef PLANET_HPP
#define PLANET_HPP

#include <vector>
#include <random>
#include <iostream>
#include "celestial_body.hpp"

struct Planet : public Celestial_body{

    void set_mass(){
        mass = 1000;
    }

};

#endif // PLANET_HPP