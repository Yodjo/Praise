#include <iostream>

#include <cmath>
#include <list>
#include <map>
#include <vector>


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "include/Path.h"
#include "include/const.h"
#include "include/noisegenerator.h"

using namespace std;


int main()
{
    cout << "Praise v0.39.4" << endl;

    NoiseGenerator nGene;

    //Tu donnes une valeur racine (n'importe quel nombre) et tu mélanges la table
    nGene.SetNewSeed(time(NULL));
    nGene.ShufflePermutationTable();
    //Le générateur est prêt à être utilisé

    sf::Image HeightMap;
    HeightMap.create(300,300);

    float resolution = 100.0f;//A augmenter ou diminuer selon la taille des bosquets que tu veux

    for(int i(0) ; i<300; i++)
    {
            for(int j(0) ; j<300; j++)
            {
                float valeur = nGene.Get2DPerlinNoiseValue(i,j,resolution);
                valeur = (valeur+1)*255/2.f;

                if(valeur >= 175)
                HeightMap.setPixel(j, i, sf::Color(valeur, valeur, valeur));
                else if(valeur >= 125)
                HeightMap.setPixel(j, i, sf::Color(150, 255, 150));
                else if(valeur >= 115 && valeur < 125)
                HeightMap.setPixel(j, i, sf::Color(255, 200, valeur));
                else
                HeightMap.setPixel(j, i, sf::Color(valeur, valeur, 255));


            }
    }

    /*cout << "Pathfinding" << endl;

    vector<vector<int> > WalkMap;

    for(int y = 0; y < 300; y++)
    {
        WalkMap.push_back(vector<int>());

        for(int x = 0; x < 300; x++)
        {
            WalkMap[y].push_back(int(0));
        }
    }

    Path::SetMap(&WalkMap);

    vector<pair<int,int> > Chemin;

    sf::Thread PathThread(&Path::GetPathTread, Treaded(&Chemin, pair<int, int>(0,0), pair<int, int>(299, 299)) );
    PathThread.launch();

    int ct = 0;
    while(Chemin.size() == 0)
    {
        cout << "\r recherche du chemin : " << ct;

        ct++;
    }

    cout << endl;

    for(vector<pair<int, int> >::iterator it = Chemin.begin(); it != Chemin.end(); it++)
    {
        cout << "- Tile(" << it->second << "," << it->first << ")" << endl;
    }*/

    // Create the main window
    sf::RenderWindow App(sf::VideoMode(800, 600), "Praise - v0.39.4");

    sf::Texture Hmap;
    Hmap.loadFromImage(HeightMap);
    sf::Sprite SpHmap;
    SpHmap.setTexture(Hmap);

    // Start the game loop
    while (App.isOpen())
    {
        // Process events
        sf::Event event;
        while (App.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
            App.close();
        }

        App.clear();

        App.draw(SpHmap);

        App.display();

    }

    return 0;
}

// A faire (Propriétaire#2#): Rendre le path finding meilleur (cout f, g, h, avoir le cjhemin vraimeent le plus court.

//i = y /// j = x
//prec.first = y /// prec.second = x
