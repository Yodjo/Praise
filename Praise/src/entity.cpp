#include "../include/entity.h"

using namespace std;

Entity::Entity()
{
    Civ.Nom = "Humain";
    Civ.Couleur = sf::Color(220, 200, 120);

    Cherche = false;

    Coord.x = 0;
    Coord.y = 0;

    Chemin.clear();
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

    //Deplacement sur une tile rdm
    /*if(TimingAction.getElapsedTime() >= sf::milliseconds(ACTION_TIME))
    {
        if(Chemin.size() < 1 && Cherche == false)
        {
            //cout << "Recherche lancee : " << __LINE__  << " -- "<< Chemin.size() << endl;
            sf::Vector2i Dest = World.GetWalkTile();

            TimingAction.restart();

            //Chemin = Path::GetPath(pair<int, int>(Coord.y,Coord.x), pair<int, int>(Dest.y, Dest.x));
            Cherche = true;

            return Treaded(&Chemin, pair<int, int>(Coord.y,Coord.x), pair<int, int>(Dest.y, Dest.x), &Cherche);
        }
        else if(Chemin.size() > 0)
        {
            cout << "Moving : " << __LINE__ << " -- " << Chemin.size() << endl;
            Cherche = false;

            Coord.x = Chemin[0].second;
            Coord.y = Chemin[0].first;

            Path::LockMutex();
            Chemin.erase(Chemin.begin());
            Path::UnLockMutex();

            TimingAction.restart();

            return Treaded(NULL, pair<int, int>(0, 0), pair<int, int>(0, 0), NULL);
        }
        else
        {
            //cout << "@ " << __LINE__ << " -- " << Chemin.size() << endl;
            TimingAction.restart();

            return Treaded(NULL, pair<int, int>(0, 0), pair<int, int>(0, 0), NULL);
        }


    }
    else
    return Treaded(NULL, pair<int, int>(0, 0), pair<int, int>(0, 0), NULL);*/

    //Action de base :
    if(TimingAction.getElapsedTime() >= sf::milliseconds(ACTION_TIME))
    {
        //A attendu la fin du "tour" ( ACTION_TIME millisecondes -> actuellement 200 ms)
        if(!Chemin.empty())  //Possede déjà un chemin (/objectif)
        {
            cout << "Moving : " << __LINE__ << " -- " << Chemin.size() << endl;
            Cherche = false;

            Coord.x = Chemin[0].second;
            Coord.y = Chemin[0].first;

            Path::LockMutex();
            Chemin.erase(Chemin.begin());
            Path::UnLockMutex();

            TimingAction.restart();

            return Treaded(NULL, pair<int, int>(0, 0), pair<int, int>(0, 0), NULL);
        }
        else if(Chemin.empty() && !Cherche) //Couper des arbres
        {
            cout << "TreeCuting : " << __LINE__ << " -- " << Chemin.size() << endl;

            return Ac_Cut_Tree(World);
        }
        else
        {
            cout << "Other : " << __LINE__ << " -- " << Chemin.size() << endl;
            //cout << "@ " << __LINE__ << " -- " << Chemin.size() << endl;
            TimingAction.restart();

            return Treaded(NULL, pair<int, int>(0, 0), pair<int, int>(0, 0), NULL);
        }
    }
    else
    return Treaded(NULL, pair<int, int>(0, 0), pair<int, int>(0, 0), NULL);
}

Treaded Entity::Ac_Rdm(Map World)
{
    cout << "-> Ac_Rdm " << __LINE__  << " -- "<< Chemin.size() << endl;
    sf::Vector2i Dest = World.GetWalkTile();

    TimingAction.restart();

    //Chemin = Path::GetPath(pair<int, int>(Coord.y,Coord.x), pair<int, int>(Dest.y, Dest.x));
    Cherche = true;
    return Treaded(&Chemin, pair<int, int>(Coord.y,Coord.x), pair<int, int>(Dest.y, Dest.x), &Cherche);

}

Treaded Entity::Ac_Cut_Tree(Map World)
{
    //cout << "Recherche lancee (Arbre) : " << __LINE__  << " -- "<< Chemin.size() << endl;
    sf::Vector2i Dest = World.GetTree(Coord);

    TimingAction.restart();

    if(Dest.x != -1 && Dest.y != -1)
    {
        Cherche = true;
        return Treaded(&Chemin, pair<int, int>(Coord.y,Coord.x), pair<int, int>(Dest.y, Dest.x), &Cherche);
    }
    else
    {
        return Ac_Rdm(World);
    }
}
