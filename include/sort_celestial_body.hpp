#ifndef SORT_CLESTIAL_BODY_HPP
#define SORT_CELESTIAL_BODY_HPP

#include "celestial_body.hpp"

extern int GALAXY_DIMENSION;

float distance(Celestial_body *galaxy, int i, int j)
{
    return sqrt((galaxy[j].position.x - galaxy[i].position.x)*(galaxy[j].position.x - galaxy[i].position.x) + (galaxy[j].position.y - galaxy[i].position.y)*(galaxy[j].position.y - galaxy[i].position.y));
}

/**
 * @brief Sorts the Celestial bodies in order of...
 */
void sort(Celestial_body *galaxy, sf::VertexArray &points)
{

    for(int i=1; i<GALAXY_DIMENSION-1; ++i){

        if(distance(galaxy, i, 0) > distance(galaxy, i+1, 0)){
            std::swap(galaxy[i], galaxy[i+1]);
            std::swap(points[i], points[i+1]);
        }

    }

}

void sort_all(Celestial_body *galaxy, sf::VertexArray &points)
{

    for(int i=0; i<GALAXY_DIMENSION-1; ++i){

        for(int j=i+1; j<GALAXY_DIMENSION; ++j){

            if(distance(galaxy, i, 0) > distance(galaxy, j, 0)){
                std::swap(galaxy[i], galaxy[j]);
                std::swap(points[i], points[j]);
            }

        }

    }

}

#endif // SORT_CELESTIAL_BODY_HPP