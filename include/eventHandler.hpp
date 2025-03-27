#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <iostream>
#include "SFML/Graphics.hpp"

void EventHandler(std::optional<sf::Event> event, sf::View &view, sf::Window &window){

    if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>()){
        
        if(mouseWheelScrolled->delta > 0){
            view.zoom(0.5);
            // std::cout << "wheel type: vertical" << std::endl;

        }else{
            view.zoom(2);
            // std::cout << "wheel type: horizontal" << std::endl;
        }
    }

    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
        
        if (keyPressed->scancode == sf::Keyboard::Scan::U){

            view.zoom(2);
            // std::cout << "zoom up" << std::endl;
        }

        if (keyPressed->scancode == sf::Keyboard::Scan::D){

            view.zoom(0.5);
            // std::cout << "zoom down" << std::endl;
        }
    }
    
    if (event->is<sf::Event::Closed>()){
        window.close();
    }

}

#endif // EVENTHENDLER_HPP