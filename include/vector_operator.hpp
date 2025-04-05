#ifndef MATH_OPERATOR_HPP
#define MATH_OPERATOR_HPP

#include "SFML/Graphics.hpp"

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

template<typename T>
sf::Vector2f operator /(const sf::Vector2f left, const sf::Vector2f right)
{
    T X = left.x / right.x;
    T Y = left.y / right.y;
    return sf::Vector2f(X,Y);
}

sf::Vector2f operator /(const sf::Vector2f left, const float right)
{
    float X = left.x / right;
    float Y = left.y / right;
    return sf::Vector2f(X,Y);
}

#endif