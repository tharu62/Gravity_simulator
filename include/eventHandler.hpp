#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include <iostream>
#include "SFML/Graphics.hpp"


void EventHandler(std::optional<sf::Event> event, sf::View &view, sf::RenderWindow &window, sf::Vector2f &oldPos, bool &moving){

    if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>()){
        
        if(mouseWheelScrolled->delta > 0){
            view.zoom(0.5);
            // std::cout << "wheel type: vertical" << std::endl;

        }else{
            view.zoom(2);
            // std::cout << "wheel type: horizontal" << std::endl;
        }
    }

    if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()){
        moving = true;
        oldPos = window.mapPixelToCoords(sf::Vector2i(mouseButtonPressed->position.x, mouseButtonPressed->position.y));
    }

    if(const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()){
        moving = false;
    }

    if(const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()){
        
        if (moving){
            const sf::Vector2f newPos = window.mapPixelToCoords(sf::Vector2i(mouseMoved->position.x, mouseMoved->position.y));
            const sf::Vector2f deltaPos = oldPos - newPos;
            view.setCenter(view.getCenter() + deltaPos);
            window.setView(view);
            oldPos = window.mapPixelToCoords(sf::Vector2i(mouseMoved->position.x, mouseMoved->position.y));
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