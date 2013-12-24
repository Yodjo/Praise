#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <vector>

#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

//TOdo : corriger kes entité, le path finding ect ...

#include "Path.h"
#include "const.h"
#include "Stock.h"
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
        void Action(Map &World);
        void Ac_Cut_Tree(Map &World);
        void Ac_Rdm(Map &World);
        void Ac_Find_Tree(Map &World);

        bool IsNextTo(sf::Vector2i &Next, Map &World);

        /** Entity affichage */
        void draw(sf::RenderWindow &App);
    protected:
    private:
        sf::Clock TimingAction;
        CivInfo Civ;
        sf::Vector2i Coord;
        std::vector< std::pair<int,int> > Chemin;
        bool Cherche;
        int ActionLoop;
        int SubActionLoop;
        std::vector< sf::Vector2i > waitingArbre;

        sf::Rect<int> *TerrainMaison;

        Stock Inventaire;

};

#endif // ENTITY_H
