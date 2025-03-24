#ifndef PLANET_HPP
#define PLANET_HPP

#include <vector>
#include <random>
#include <iostream>
#include "celestial_body.hpp"

class Planet : public Celestial_body{

    private:

    public:

    void set_random_color(){
        // the planets should have a dark color like dark blue, dark green, brown ecc...
        set_color(rand() % 255, rand() % 255, rand() % 255);
    }

};

#endif // PLANET_HPP