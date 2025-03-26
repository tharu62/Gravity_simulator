#ifndef PLANET_HPP
#define PLANET_HPP

#include <vector>
#include <random>
#include <iostream>
#include "celestial_body.hpp"

class Planet : public Celestial_body{

    private:

    public:
    Planet(){
        type = planet;
    }

    /**
     * @brief the planets should have a dark color like dark blue, dark green, brown ecc...
     */
    void set_color(){ 
        Celestial_body::set_color(255, 0, 255);
    }

    void set_mass(){
        Celestial_body::set_mass(1);
    }

};

#endif // PLANET_HPP