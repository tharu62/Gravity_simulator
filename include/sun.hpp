#ifndef SUN_HPP
#define SUN_HPP

#include <vector>
#include <random>
#include <iostream>
#include "celestial_body.hpp"

class Sun : public Celestial_body{

    private:

    public:

    void set_mass(){
        Celestial_body::set_mass(333000);
    }

};

#endif // SUN_HPP