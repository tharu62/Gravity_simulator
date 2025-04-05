#ifndef BLACK_HOLE_HPP
#define BLACK_HOLE_HPP

#include <vector>
#include <random>
#include <iostream>
#include "celestial_body.hpp"

struct Black_hole : public Celestial_body{

    // int event_horizon;

    void set_mass(){
        mass = 1265400000;
    }   

    // void set_event_horizon(double r){
    //     // the event horizon is the point of no return
    //     // the radius of the black hole should be smaller than the event horizon
    //     event_horizon = r*2;
    // }

};

#endif // BLACK_HOLE_HPP
