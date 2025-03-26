#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <iostream>
#include "SFML/Graphics.hpp"

void EventHandler(std::optional<sf::Event> event){

    if(event->is<sf::Event::MouseWheelScrolled>()){
        std::cout << "mouse is scrolling" << std::endl;
    }

}

#endif // EVENTHENDLER_HPP