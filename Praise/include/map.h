#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "const.h"
#include "noisegenerator.h"

class Map
{
    public:
        /** Default constructor */
        Map();
        /** Default destructor */
        virtual ~Map();

        void Gen(int TailleX, int TailleY);
        void affMiniMap(sf::RenderWindow &App);
        void MajWalkRdm();

        /** getter */
        std::vector< std::vector<int> > GetWalkMap();
        sf::Vector2i GetWalkTile();
        sf::Vector2i GetTree(sf::Vector2i pos);
        int isWalkable(sf::Vector2i pos);


    protected:
    private:
        std::vector< std::vector<int> > TiledMap;
        std::vector< std::vector<int> > WalkMap;
        std::vector< std::vector<Arbre> > TreeMap;
        std::vector<sf::Vector2i> RdmTile;

};

#endif // MAP_H
