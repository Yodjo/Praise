#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

//TOdo : corriger kes entité, le path finding ect ...

#include "Path.h"
#include "const.h"
#include "map.h"

class Entity
{
    public:
        /** Default constructor */
        Entity();
        Entity(sf::Vector2i nCoord);
        Entity(CivInfo nCiv);
        Entity(sf::Vector2i nCoord, CivInfo nCiv);
        /** Default destructor */
        virtual ~Entity();

        /** Entity action */
        Treaded Action(Map World);

        /** Entity affichage */
        void draw(sf::RenderWindow &App);
    protected:
    private:
        sf::Clock TimingAction;
        CivInfo Civ;
        sf::Vector2i Coord;
        std::vector< std::pair<int,int> > Chemin;
        bool Cherche;

};

#endif // ENTITY_H
