#ifndef BLACK_HOLE_HPP
#define BLACK_HOLE_HPP

#include <vector>
#include <random>
#include <iostream>
#include "celestial_body.hpp"

class Black_hole : public Celestial_body{

    private:
    int event_horizon;

    public:
    Black_hole(){
        type = black_hole;
    }

    void set_color(){
        // the black hole should have a dark color like black
        Celestial_body::set_color(0, 0, 0);
    }

    void set_mass(){
        Celestial_body::set_mass(1);
    }

    void set_event_horizon(double r){
        // the event horizon is the point of no return
        // the radius of the black hole should be smaller than the event horizon
        event_horizon = r*2;
    }

};

#endif // BLACK_HOLE_HPP
