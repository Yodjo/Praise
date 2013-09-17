#include <iostream>
#include <vector>


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "include/Path.h"
#include "include/const.h"
#include "include/map.h"


using namespace std;


int main()
{
    cout << "Praise v0.39.4" << endl;
    srand(time(NULL));

    Map WorldMap;
    WorldMap.Gen(300, 300);

    cout << "Pathfinding" << endl;

    vector< vector<int> > WalkMap;
    WalkMap = WorldMap.GetWalkMap();
    Path::SetMap(&WalkMap);

    vector<pair<int,int> > Chemin;

    sf::Thread PathThread(&Path::GetPathTread, Treaded(&Chemin, pair<int, int>(0,0), pair<int, int>(299, 299)) );
    PathThread.launch();

    cout << endl;

    for(vector<pair<int, int> >::iterator it = Chemin.begin(); it != Chemin.end(); it++)
    {
        cout << "- Tile(" << it->second << "," << it->first << ")" << endl;
    }

    ///JEU EN LUI MEME

    // Create the main window
    sf::RenderWindow App(sf::VideoMode(960, 540), "Praise - v0.39.4");

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

        WorldMap.affMiniMap(App);

        App.display();

    }

    return 0;
}

// A faire (Propriétaire#2#): Rendre le path finding meilleur (cout f, g, h, avoir le cjhemin vraimeent le plus court.

//i = y /// j = x
//prec.first = y /// prec.second = x
