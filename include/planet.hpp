#ifndef PLANET_HPP
#define PLANET_HPP

#include <vector>
#include <random>
#include <iostream>
#include "celestial_body.hpp"

class Planet : public Celestial_body{

    private:

    public:

    void set_mass(){

        Celestial_body::set_mass(1);
    }

};

#endif // PLANET_HPP