#ifndef MATH_OPERATOR_HPP
#define MATH_OPERATOR_HPP

#include "SFML/Graphics.hpp"
#include <cmath>

// template<typename T>
sf::Vector2f operator *(sf::Vector2f left, int right){
    int X = left.x * right;
    int Y = left.y * right;
    return sf::Vector2f((float)X,(float)Y);
}

sf::Vector2f operator *(sf::Vector2f left, float right){
    float X = left.x * right;
    float Y = left.y * right;
    return sf::Vector2f(X,Y);
} 

sf::Vector2f operator *(sf::Vector2f left, double right){
    double X = left.x * right;
    double Y = left.y * right;
    return sf::Vector2f((float) X,(float) Y);
} 

sf::Vector2f operator *(sf::Vector2f left, unsigned int right){
    float X = left.x * right;
    float Y = left.y * right;
    return sf::Vector2f(X,Y);
}

sf::Vector2f operator *(sf::Vector2f left, unsigned long int right){
    float X = left.x * right;
    float Y = left.y * right;
    return sf::Vector2f(X,Y);
}

sf::Vector2f operator *(sf::Vector2f left, long long int right){
    float X = left.x * right;
    float Y = left.y * right;
    return sf::Vector2f(X,Y);
}

sf::Vector2f operator *(sf::Vector2f left, long double right){
    long double X = left.x * right;
    long double Y = left.y * right;
    return sf::Vector2f(X,Y);
}

template<typename T>
sf::Vector2f operator /(const sf::Vector2f left, const sf::Vector2f right)
{
    if(right.x == 0 || right.y == 0) return left;    
    T X = left.x / right.x;
    T Y = left.y / right.y;
    return sf::Vector2f(X,Y);
}

sf::Vector2f operator /(const sf::Vector2f left, const float right)
{
    if(right == 0) return left;
    float temp = 1.f / right;
    float X = left.x * temp;
    float Y = left.y * temp;
    return sf::Vector2f(X,Y);
}

sf::Vector2f operator /(const sf::Vector2f left, const double right)
{
    if(right == 0) return left;
    double temp = 1.0 / right;
    double X = left.x * temp;
    double Y = left.y * temp;
    return sf::Vector2f((float)X,(float)Y);
}

sf::Vector2f operator /(const sf::Vector2f left, const long double right)
{
    if(right == 0) return left;
    long double temp = 1.f / right;
    long double X = left.x * temp;
    long double Y = left.y * temp;
    return sf::Vector2f(X,Y);
}

float sign(float arg)
{
    if(arg > 0.f) return 1.f;
    else if(arg < 0.f) return -1.f;
    else return 0.f;
}

float log_safe(float arg)
{
    if(arg <= 1.f && arg >= -1.f){
        return 1.f*sign(arg);
    } 
    else {
        return (float)log((float)abs(arg))*sign(arg);
    }
}

sf::Vector2f log(sf::Vector2f arg)
{
    float X;
    float Y;

    // if(arg.x < -1.f){
    //     X = -1.f*log(-1.f*arg.x);
    // } 
    // else if(arg.x > 1.f){
    //     X = (float)log(arg.x);
    // }
    // else if(arg.x > -1.f && arg.x < 1.f){
    //     X = arg.x;
    // }
    // else{
    //     X = 0.f;
    // }

    // if(arg.y < -1.f){
    //     Y = -1.f*log(-1.f*arg.y);
    // } 
    // else if(arg.y > 1.f){
    //     Y = (float)log(arg.y);
    // }
    // else if(arg.y > -1.f && arg.y < 1.f){
    //     Y = arg.y;
    // }
    // else{
    //     Y = 0.f;
    // }
    if(arg.x < 0.f){
        X = -1.f*log(-1.f*arg.x + 1.f);
    } 
    else if(arg.x > 0.f){
        X = (float)log(arg.x + 1.f);
    }
    else{
        X = 0.f;
    }

    if(arg.y < 0.f){
        Y = -1.f*log(-1.f*arg.y + 1.f);
    } 
    else if(arg.y > 0.f){
        Y = (float)log(arg.y + 1.f);
    }
    else{
        Y = 0.f;
    }
    return sf::Vector2f(X,Y);
}

#endif