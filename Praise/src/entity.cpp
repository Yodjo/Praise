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

    ActionLoop = CUT_TREE;
    SubActionLoop = TROUVE_ARBRE;
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

void Entity::Action(Map &World)
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
            //Si -1/-1 alors pas de chemin trouver pour un arbre (-1) et donc suppression du chemin et de l'action (arbre)
            if(Chemin[0].second == -1 && Chemin[0].first == -1)
            {
               // cout << "+ pas de chemin : suppr l'arbre de la list : " << __LINE__ << endl;
                waitingArbre.erase(waitingArbre.begin());
                Chemin.clear();
            }
            else
            {
                //On se déplace et on mets a jour le chemin

                ///       cout << "Moving : " << __LINE__ << " -- " << Chemin.size() << endl;
                Cherche = false;

                Coord.x = Chemin[0].second;
                Coord.y = Chemin[0].first;

                Chemin.erase(Chemin.begin());

                TimingAction.restart();

            }

           // return Treaded(NULL, pair<int, int>(0, 0), pair<int, int>(0, 0), NULL);


        }
        else if(Chemin.empty() && !Cherche) //Pas de chemin donc on attend de choisir une action
        {
            //cout << "TreeCuting : " << __LINE__ << " -- " << Chemin.size() << endl;
// A faire (Propriétaire#9#): Faire que le chemin soit chercher a cahque fois en fonctikn de larbre le plus proche choisi + faire que  y est une action deplacement

            switch(ActionLoop)
            {
                case CUT_TREE:  //Action de couper un arbre :
                {
                    ///cout << "Couper Abre : " << __LINE__ << " -- ";
                    if(waitingArbre.empty())    // - Si on a pas d'arbres en attente : on en ajoute
                    {
                     ///  cout << "+ Ajouter un arbre : " << __LINE__ << endl;
                        Ac_Find_Tree(World);
                    }
                    else if(!waitingArbre.empty() && Chemin.empty() && !Cherche) // - Si on a un/plusieurs arbres en attente : on les choisi ( le premier )
                    {

                        if(IsNextTo(waitingArbre[0], World)) //Si on est a coté de l'arbre 0 (le premier) alors on la coupe
                        {
                          ///  cout << "+ Couper l'arbre : " << __LINE__ << /*" - PosArbre : " << waitingArbre[0].x << " " << waitingArbre[0].x  << " + PosEnt :" << Coord.x << " " << Coord.y <<*/ endl;
                            World.CutTree(waitingArbre[0]);
                            waitingArbre.erase(waitingArbre.begin());
                        }
                        else //Sinon cherche le chemin vers le premier arbre.
                        {
                         ///   cout << "+ Chercher le chemin vers un arbre : " << __LINE__ << endl;
                            Ac_Cut_Tree(World);
                        }
                    }
                }
                break;
            }

        }
        else
        {
            //cout << "Other : " << __LINE__ << " -- " << Chemin.size() << endl;
            //cout << "@ " << __LINE__ << " -- " << Chemin.size() << endl;
            TimingAction.restart();

            //return Treaded(NULL, pair<int, int>(0, 0), pair<int, int>(0, 0), NULL);
        }

    }

    //else
    //return Treaded(NULL, pair<int, int>(0, 0), pair<int, int>(0, 0), NULL);
}

//On regarde si l'entité est a coté de l'objet (a une case près)
bool Entity::IsNextTo(sf::Vector2i Next, Map &World)
{
    for(int x = -1; x < 2; x++)
    {
        if(x + Coord.x < 0 || x+Coord.x > World.getSizeX())
            continue;

        for(int y = -1; y < 2; y++)
        {
            if(y+Coord.y < 0 ||y+Coord.y > World.getSizeY())
                continue;

            if(Next.x == x+Coord.x && Next.y == y+Coord.y)
                return true;
        }
    }

    return false;
}

//Action : se deplacé au hasard si on a pas d'action, ou pas de possibilité d'action.
void Entity::Ac_Rdm(Map &World)
{

    //cout << "->" /*Ac_Rdm "*/ << __LINE__  << " -- "<< Chemin.size() << endl;
    sf::Vector2i Dest = World.GetWalkTile();

    TimingAction.restart();

    //Chemin = Path::GetPath(pair<int, int>(Coord.y,Coord.x), pair<int, int>(Dest.y, Dest.x));
    Cherche = true;
    Treaded A = Treaded(&Chemin, pair<int, int>(Coord.y,Coord.x), pair<int, int>(Dest.y, Dest.x), &Cherche);
    Path::AddPathTask(A);

}

//Action de trouver un arbre
void Entity::Ac_Find_Tree(Map &World)
{
    //On prend un arbre au hasard
    sf::Vector2i Tree = World.GetTree(Coord, VIEW_SIZE);

    TimingAction.restart();
    int a = 0;

    //On verifie si il est accecible
    if(Tree.x != -1 && Tree.y != -1)
    {
        for(int TreeX = -1; TreeX < 2; ++TreeX)
        {
            for(int TreeY = -1; TreeY < 2; ++TreeY)
            {
                if(TreeX == 0 && TreeY == 0)
                    continue;

                if(World.isWalkable(Tree.x + TreeX, Tree.y + TreeY) == W_OUI)
                {
                    waitingArbre.push_back(Tree);

                    TreeY=3;
                    TreeX=3;
                    a++;
                    cout << 'a' << endl;
                }
                break;

            }
        }
    }
    else
    {
        Ac_Rdm(World);
    }

}

//Verif : useless ...
void Entity::Ac_Cut_Tree(Map &World)
{
    //cout << "Recherche lancee (Arbre) : " << __LINE__  << " -- "<< Chemin.size() << endl;
    if(!waitingArbre.empty())
    {
        sf::Vector2i Dest(-1, -1);

        for(int TreeX = -1; TreeX < 2; ++TreeX)
        {
            for(int TreeY = -1; TreeY < 2; ++TreeY)
            {
                if(TreeX == 0 && TreeY == 0)
                    continue;

                if(World.isWalkable(waitingArbre[0].x + TreeX, waitingArbre[0].y + TreeY) == W_OUI)
                {
                    Dest.x = TreeX + waitingArbre[0].x;
                    Dest.y = TreeY + waitingArbre[0].y;

                    TreeY=3;
                    TreeX=3;

                }
                break;

            }
        }

        if(Dest.x != -1 && Dest.y != -1)
        {
            TimingAction.restart();

            //cout << "-> Position choisi : " << __LINE__  << " : " << Dest.x << " " << Dest.y << " walakble : " << World.isWalkable(Dest.x, Dest.y) <<endl;

            if(sqrt((Dest.x-Coord.x)*(Dest.x-Coord.x) + (Dest.y-Coord.y)*(Dest.y-Coord.y)) < 5)
            Chemin = Path::GetPath(pair<int, int>(Coord.y,Coord.x), pair<int, int>(Dest.y, Dest.x));
            else
            {
            Cherche = true;
            Treaded A = Treaded(&Chemin, pair<int, int>(Coord.y,Coord.x), pair<int, int>(Dest.y, Dest.x), &Cherche);
            Path::AddPathTask(A);
            }

            SubActionLoop = COUPE_ARBRE;
        }
        else
        Ac_Rdm(World);
    }
    else
    Ac_Rdm(World);

}
