#ifndef CELESTIAL_BODY_HPP
#define CELESTIAL_BODY_HPP

#include <vector>

class Celestial_body
{   
    private:
    double mass;
    double radius;
    std::vector<int> color;
    std::vector<double> position;    
    std::vector<double> velocity;
    std::vector<double> acceleration;

    public:
    Celestial_body(){
        mass = 0;
        radius = 0;
        position = {0,0,0};
        velocity = {0,0,0};
        acceleration = {0,0,0};
    }

    void set_mass(double m){
        mass = m;
    }
    
    void set_radius(double r){
        radius = r;
    }

    void set_color(int r, int g, int b){
        color = {r, g, b};
    }

    void set_position(std::vector<double> p){
        position = p;
    }

    void set_velocity(std::vector<double> v){
        velocity = v;
    }

    void set_acceleration(std::vector<double> a){
        acceleration = a;
    }

    double get_mass(){
        return mass;
    }

    double get_radius(){
        return radius;
    }

    std::vector<int> get_color(){
        return color;
    }

    std::vector<double> get_position(){
        return position;
    }

    std::vector<double> get_velocity(){
        return velocity;
    }

    std::vector<double> get_acceleration(){
        return acceleration;
    }

};

#endif // CELESTIAL_BODY_HPP