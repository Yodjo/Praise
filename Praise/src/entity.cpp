#include "../include/entity.h"

using namespace std;

Entity::Entity()
{
    Civ.Nom = "Humain";
    Civ.Couleur = sf::Color(/*220, 200, 120*/255,000,000);

    Cherche = false;

    Coord.x = 0;
    Coord.y = 0;

    Chemin.clear();

    ActionLoop = NOTHING;
    SubActionLoop = TROUVE_ARBRE;

    TerrainMaison = NULL;
}

Entity::Entity(sf::Vector2i nCoord)
{
    Civ.Nom = "Humain";
    Civ.Couleur = sf::Color(/*220, 200, 120*/255,000,000);

    Coord.x = nCoord.x;
    Coord.y = nCoord.y;

    ActionLoop = NOTHING;
    SubActionLoop = TROUVE_ARBRE;

    TerrainMaison = NULL;
}

Entity::Entity(CivInfo nCiv)
{
    Civ = nCiv;

    Coord.x = 0;
    Coord.y = 0;

    ActionLoop = NOTHING;
    SubActionLoop = TROUVE_ARBRE;

    TerrainMaison = NULL;
}

Entity::Entity(sf::Vector2i nCoord, CivInfo nCiv)
{
    Civ = nCiv;

    Coord.x = nCoord.x;
    Coord.y = nCoord.y;

    ActionLoop = NOTHING;
    SubActionLoop = TROUVE_ARBRE;

    TerrainMaison = NULL;
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
    //Action de base :
    if(TimingAction.getElapsedTime() >= sf::milliseconds(ACTION_TIME))
    {
         //A attendu la fin du "tour" ( ACTION_TIME millisecondes -> actuellement 200 ms)

        if(ActionLoop == NOTHING && !Cherche)
        {

            if(!waitingArbre.empty())
            ActionLoop = CUT_TREE;
            else
            ActionLoop = BUILD_HOUSE;
        }

        if(!Chemin.empty() && !Cherche)  //Possede déjà un chemin (/objectif)
        {
            //Si -1/-1 alors pas de chemin trouver pour un arbre (-1) et donc suppression du chemin et de l'action (arbre)
            if(Chemin[0].second == -1 && Chemin[0].first == -1)
            {
               // if(!waitingArbre.empty() && ActionLoop == CUT_TREE)
                //{
                    //waitingArbre.erase(waitingArbre.begin());
                    ActionLoop = NOTHING;
                //}

                Chemin.clear();
            }
            else
            {
                //On se déplace et on mets a jour le chemin
                Cherche = false;

                Coord.x = Chemin[0].second;
                Coord.y = Chemin[0].first;

                Chemin.erase(Chemin.begin());

                TimingAction.restart();

            }

        }
        else if(Chemin.empty() && !Cherche) //Pas de chemin donc on attend de choisir une action
        {
            switch(ActionLoop)
            {
                case CUT_TREE:  //Action de couper un arbre :
                {
                    if(waitingArbre.empty())    // - Si on a pas d'arbres en attente : on en ajoute
                    {
                        Ac_Find_Tree(World);
                    }
                    else if(!waitingArbre.empty()) // - Si on a un/plusieurs arbres en attente : on les choisi ( le premier )
                    {
                        if(IsNextTo(waitingArbre[0], World)) //Si on est a coté de l'arbre 0 (le premier) alors on la coupe
                        {
                            World.CutTree(waitingArbre[0]);
                            Inventaire.Add(BUCHE);
                            waitingArbre.erase(waitingArbre.begin());

                            Inventaire.Voir();
                        }
                        else //Sinon cherche le chemin vers le premier arbre.
                        {
                            Ac_Cut_Tree(World);
                        }
                    }

                    ActionLoop = NOTHING;
                }
                break;

                case BUILD_HOUSE:
                {
                    // 1 choisir un endroit:
                        //Pas trop loin du la position actuelle :
                    if(Cherche == false)
                    {

                    if(TerrainMaison == NULL)
                    {
                        sf::Vector2i PositionMaisonNew(Coord.x+rand()%20-10, Coord.y+rand()%20-10);
                        TerrainMaison = World.addTerrain(PositionMaisonNew, 3);

                        if(TerrainMaison == NULL)
                        Ac_Rdm(World);

                    }
                    else
                    {
                        // 2 couper les arbres
                        if(World.DefrichTerrain(TerrainMaison, &waitingArbre))
                        {
                            //3 construire la maison
                            if(Coord != sf::Vector2i(TerrainMaison->left, TerrainMaison->top))
                            {
                                Cherche = true;
                                Treaded A = Treaded(&Chemin, pair<int, int>(Coord.y,Coord.x), pair<int, int>(TerrainMaison->top, TerrainMaison->left), &Cherche);
                                Path::AddPathTask(A);
                            }
                            else
                            Ac_Rdm(World);
                        }

                        //cout << "Waitingarbre size : " << waitingArbre.size() << endl;

                    }

                    }

                    ActionLoop = NOTHING;
                }
                break;
            }

        }
        else
        {
            TimingAction.restart();
        }
    }
}

//On regarde si l'entité est a coté de l'objet (a une case près)
bool Entity::IsNextTo(sf::Vector2i &Next, Map &World)
{
    for(int x = -1; x < 2; x++)
    {
        if(x + Coord.x < 0 || x + Coord.x > World.getSizeX())
            continue;

        for(int y = -1; y < 2; y++)
        {
            if(y + Coord.y < 0 ||y + Coord.y > World.getSizeY())
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

                    //cout << "Pos Tree X : " << waitingArbre[0].x << " et Pos Tree Y : " << waitingArbre[0].y << endl;

                    TreeY=3;
                    TreeX=3;

                }
                break;

            }
        }

        if(Dest.x != -1 || Dest.y != -1)
        {
            TimingAction.restart();

            //cout << "-> Position choisi : " << __LINE__  << " : " << Dest.x << " " << Dest.y << " walkable : " << World.isWalkable(Dest.x, Dest.y) <<endl;
            //faire que ce soit dans une autre boucle de recherche les truk proche
            Cherche = true;
            Treaded A = Treaded(&Chemin, pair<int, int>(Coord.y,Coord.x), pair<int, int>(Dest.y, Dest.x), &Cherche);
            Path::AddPathTask(A);

            SubActionLoop = COUPE_ARBRE;
        }
        else
        Ac_Rdm(World);
    }
    else
    Ac_Rdm(World);

}
