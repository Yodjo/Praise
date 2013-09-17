#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>


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

        /** getter */
        std::vector< std::vector<int> > GetWalkMap();
    protected:
    private:
        std::vector< std::vector<int> > TiledMap;
        std::vector< std::vector<int> > WalkMap;
        std::vector< std::vector<Arbre> > TreeMap;

};

#endif // MAP_H
