#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <iostream>
#include "SFML/Graphics.hpp"

void EventHandler(std::optional<sf::Event> event, sf::View &view){

    if(event->is<sf::Event::MouseWheelScrolled>()){
        view.zoom(-2);
        std::cout << "zoom down" << std::endl;
    }
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
        
        if (keyPressed->scancode == sf::Keyboard::Scan::U){

            view.zoom(2);
            std::cout << "zoom up" << std::endl;
        }

        if (keyPressed->scancode == sf::Keyboard::Scan::D){

            view.zoom(0.5);
            std::cout << "zoom up" << std::endl;
        }
    }

}

#endif // EVENTHENDLER_HPP