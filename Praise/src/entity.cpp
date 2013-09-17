#include "../include/entity.h"

using namespace std;

Entity::Entity()
{
    Civ.Nom = "Humain";
    Civ.Couleur = sf::Color(220, 200, 120);

    Coord.x = 0;
    Coord.y = 0;
}

Entity::Entity(sf::Vector2i nCoord)
{
    Civ.Nom = "Humain";
    Civ.Couleur = sf::Color(220, 200, 120);

    Coord.x = nCoord.x;
    Coord.y = nCoord.y;
}

Entity::Entity(CivInfo nCiv)
{
    Civ = nCiv;

    Coord.x = 0;
    Coord.y = 0;
}

Entity::Entity(sf::Vector2i nCoord, CivInfo nCiv)
{
    Civ = nCiv;

    Coord.x = nCoord.x;
    Coord.y = nCoord.y;
}

Entity::~Entity()
{
    //dtor
}

void Entity::draw(sf::RenderWindow &App)
{
    sf::Image ImgEnt;
    ImgEnt.create(1, 1, Civ.Couleur);

    sf::Texture TxEnt;
    TxEnt.loadFromImage(ImgEnt);

    sf::Sprite SpEnt;
    SpEnt.setTexture(TxEnt);
    SpEnt.setPosition(Coord.x, Coord.y);

    App.draw(SpEnt);
}

Treaded Entity::Action(Map World)
{
    if(Chemin.size() < 1)
    {
        sf::Vector2i Dest = World.GetWalkTile();

        return Treaded(&Chemin, pair<int, int>(Coord.x,Coord.y), pair<int, int>(Dest.x, Dest.y));
    }
    else
    {
        Coord.x = Chemin[0].second;
        Coord.y = Chemin[0].first;
        Chemin.erase(Chemin.begin());

        return Treaded(&Chemin, pair<int, int>(0, 0), pair<int, int>(0, 0));
    }

}
