#include "../include/map.h"

using namespace std;

Map::Map()
{
    //ctor
}

Map::~Map()
{
    //dtor
}

void Map::Gen(int TailleX, int TailleY)
{
    //Génération de la map (0)
    for(int j = 0; j < TailleY; j++)
    {
        TiledMap.push_back( vector<int>() );
        WalkMap.push_back( vector<int>() );
        TreeMap.push_back( vector<Arbre>() );

        for(int i = 0; i < TailleX; i++)
        {
            TiledMap[j].push_back(0);
            WalkMap[j].push_back(0);
            TreeMap[j].push_back(Arbre());
        }
    }

    NoiseGenerator nGene;

    //Tu donnes une valeur racine (n'importe quel nombre) et tu mélanges la table
    nGene.SetNewSeed(time(NULL));
    nGene.ShufflePermutationTable();
    //Le générateur est prêt à être utilisé

    float resolution = 150.0f;//A augmenter ou diminuer selon la taille des bosquets que tu veux

    for(int j(0) ; j < TailleY; j++)
    {
            for(int i(0) ; i < TailleX; i++)
            {
                float valeur = nGene.Get2DFBMNoiseValue(i,j,resolution);

                valeur = (valeur+1)*255/2.f;

                if(valeur >= 220) // pic enneigé
                {
                    TiledMap[j][i] = PIC;
                    WalkMap[j][i] = W_NON;
                }
                else if(valeur >= 190) //Hauteur élevé : montagne
                {
                    TiledMap[j][i] = MONTAGNE;
                    WalkMap[j][i] = W_NON;
                }
                else if(valeur >= 125)  //Hauteur normail : sol/plaine
                {
                    TiledMap[j][i] = PLAINE;
                    WalkMap[j][i] = W_OUI;
                }
                else if(valeur >= 115 && valeur < 125) //Une auteur suffisante  / bordure de mer : sable
                {
                    TiledMap[j][i] = PLAGE;
                    WalkMap[j][i] = W_OUI;
                }
                else    //Defaut : eau
                {
                    TiledMap[j][i] = EAU;
                    WalkMap[j][i] = W_EAU;
                }


            }
    }

    NoiseGenerator nTree;

    //Tu donnes une valeur racine (n'importe quel nombre) et tu mélanges la table
    nTree.SetNewSeed(time(NULL)+1);
    nTree.ShufflePermutationTable();
    //Le générateur est prêt à être utilisé

    float Tresolution = 60.0f;//A augmenter ou diminuer selon la taille des bosquets que tu veux

    for(int j(0) ; j < TailleY; j++)
    {
            for(int i(0) ; i < TailleX; i++)
            {
                float valeur = nTree.Get2DFBMNoiseValue(i,j,Tresolution);

                if(valeur > 0.1f && TiledMap[j][i] == PLAINE)
                {
                    if(TreeMap[max(j-1, 0)][i].Type == -1 && TreeMap[min(j+1,TailleY-1)][i].Type == -1 && TreeMap[j][max(i-1,0)].Type == -1 && TreeMap[j][min(i+1, TailleX-1)].Type == -1)
                    {
                        if((rand() % 2) == 1)
                        {
                            TreeMap[j][i].Type = POMMIER;
                            TreeMap[j][i].Taille = (rand() % TAILLE_ARBRE_MAX)+1;
                            WalkMap[j][i] = W_TREE;
                        }
                    }
                }
            }
    }

    MajWalkRdm();
}

void Map::affMiniMap(sf::RenderWindow &App)
{
    sf::Image imgMap;
    imgMap.create(TiledMap[0].size(), TiledMap.size());

    for(int j = 0; j<TiledMap.size(); j++)
        for(int i = 0; i < TiledMap[j].size(); i++)
        {
            switch(TiledMap[j][i])
            {
                case EAU:
                imgMap.setPixel(i, j, sf::Color(175, 200, 255));
                break;

                case PLAINE:
                imgMap.setPixel(i, j, sf::Color(150, 200, 150));
                break;

                case PLAGE:
                imgMap.setPixel(i, j, sf::Color(255, 255, 155));
                break;

                case MONTAGNE:
                imgMap.setPixel(i, j, sf::Color(200, 200, 200));
                break;

                case PIC:
                imgMap.setPixel(i, j, sf::Color(250, 250, 255));
                break;
            }

            if(TreeMap[j][i].Type == POMMIER)
            {
                imgMap.setPixel(i, j, sf::Color(20,100, 20));
            }
        }

    sf::Texture Txmap;
    Txmap.loadFromImage(imgMap);

    sf::Sprite SpMap;
    SpMap.setTexture(Txmap);

    SpMap.setPosition(0, 0);

    App.draw(SpMap);
}

vector< vector<int> > Map::GetWalkMap()
{
    return WalkMap;
}

void Map::MajWalkRdm()
{
    for(int j = 0; j < WalkMap.size(); j++)
    {
        for(int i = 0; i < WalkMap[j].size(); i++)
        {
            if(WalkMap[j][i] == W_OUI)
            RdmTile.push_back(sf::Vector2i(i, j));
        }
    }
}

sf::Vector2i Map::GetWalkTile()
{
    return RdmTile[rand() % RdmTile.size()];
}

sf::Vector2i Map::GetTree(const sf::Vector2i &pos, const int VueSize)
{
    vector<sf::Vector2i> PosRdmArbre;
    for(int j = -VueSize; j < VueSize; j++) //y
    {
        if(j+pos.y < 0 || j+pos.y >= TreeMap.size())
            continue;

        for(int i = -VueSize; i < VueSize; i++) //x
        {
            if(i+pos.x < 0 || i+pos.x >= TreeMap[j].size())
                continue;

            if(TreeMap[j+pos.y][i+pos.x].Type != -1)
            {
                PosRdmArbre.push_back(sf::Vector2i(i+pos.x, j+pos.y));
            }
        }
    }

    if(!PosRdmArbre.empty())
    return PosRdmArbre[rand() % PosRdmArbre.size()];
    else
    return sf::Vector2i(-1, -1);
}

int Map::isWalkable(const sf::Vector2i &pos)
{
    if(pos.x > 0 && pos.y > 0 && pos.x < WalkMap.size() && pos.y < WalkMap.size())
    return WalkMap[pos.y][pos.x];
    else
    return W_NON;
}

int Map::isWalkable(const int x,const int y)
{
    if(x > 0 && y > 0 && x < WalkMap.size() && y < WalkMap.size())
    return WalkMap[y][x];
    else
    return W_NON;
}

int Map::CutTree(const sf::Vector2i &pos)
{
    WalkMap[pos.y][pos.x] = W_OUI;
    TreeMap[pos.y][pos.x].Type = -1;

    MajWalkRdm();

    return TreeMap[pos.y][pos.x].Taille;
}

int Map::getSizeX()
{
    return TiledMap[0].size();
}

int Map::getSizeY()
{
    return TiledMap.size();
}
