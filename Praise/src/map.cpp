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

sf::Vector2i Map::GetWalkTile()
{
    vector<sf::Vector2i> RdmTile;

    for(int j = 0; j < WalkMap.size(); j++)
    {
        for(int i = 0; i < WalkMap[j].size(); i++)
        {
            if(WalkMap[j][i] == W_OUI)
            RdmTile.push_back(sf::Vector2i(i, j));
        }
    }

    return RdmTile[rand() % RdmTile.size()];
}

sf::Vector2i Map::GetTree(sf::Vector2i pos)
{
    for(int j = -VIEW_SIZE; j < VIEW_SIZE; j++) //y
    {
        if(j < 0 || j >= TreeMap.size())
            continue;

        for(int i = -VIEW_SIZE; i < VIEW_SIZE; i++) //x
        {
            if(i < 0 || i >= TreeMap[j].size())
                continue;

            if(TreeMap[j][i].Type != -1)
            {
                    cout << "a";
                    for(int Tj = -1; Tj < 2; Tj++)
                    {
                        if(j + Tj < 0 || j + Tj >= WalkMap.size())
                            continue;

                        for(int Ti = -1; Ti < 2; Tj++)
                        {
                            if(i + Ti < 0 || i + Ti >= WalkMap[j+Tj].size())
                            continue;

                            if(isWalkable(sf::Vector2i(i+Ti, j+Tj) ) == W_OUI)
                            return sf::Vector2i(i+Ti, j+Tj);
                        }
                    }

            }
        }
    }

    return sf::Vector2i(-1, -1);
}

int Map::isWalkable(sf::Vector2i pos)
{
    return WalkMap[pos.y][pos.x];
}

