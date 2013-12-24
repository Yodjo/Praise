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

// A faire (Propriétaire#5#): Faire repousser les abres
// A faire (Propriétaire#7#): faire un systeme de zone pour la construction.


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
        std::vector< std::vector<int> >* GetWalkMap();
        int getSizeX();
        int getSizeY();
        sf::Vector2i GetWalkTile();
        sf::Vector2i GetTree(const sf::Vector2i &pos, const int VueSize);
        int isWalkable(const sf::Vector2i &pos);
        int isWalkable(const int x, const int y);
        int CutTree(const sf::Vector2i &pos);

        sf::Rect<int>* addTerrain(sf::Vector2i &PositionT, int sizeWH);
        bool DefrichTerrain(sf::Rect<int> *Terrain, std::vector< sf::Vector2i > *waitingArbre);


    protected:
    private:
        std::vector< std::vector<int> > TiledMap;
        std::vector< std::vector<int> > WalkMap;
        std::vector< std::vector<Arbre> > TreeMap;
        std::vector<sf::Vector2i> RdmTile;
        std::vector< sf::Rect<int> > TerrainConstr;

};

#endif // MAP_H
