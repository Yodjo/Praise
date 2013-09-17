#include <iostream>
#include <vector>


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "include/Path.h"
#include "include/const.h"
#include "include/map.h"
#include "include/entity.h"


using namespace std;


int main()
{
    cout << "Praise v0.39.4" << endl << endl;

    cout << "Cette oeuvre, création, site ou texte est sous licence Creative Commons  Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 3.0 non transposé. Pour accéder à une copie de cette licence, merci de vous rendre à l'adresse suivante http://creativecommons.org/licenses/by-nc-sa/3.0/ ou envoyez un courrier à Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA." << endl;
    cout << endl;

    cout << "** Remerciement(s) : " << endl;
    cout <<"- Site du zero (OpenClassRooms)" << endl << "- SFML : sfml-dev.org" << endl << "- zNoise (Nazara Engine) " << endl << "- Le reste d'internet : Forum, Etc ..." << endl << endl;

    //Initialisation du hazard :
    srand(time(NULL));

    //Génération de la carte :
    Map WorldMap;
    WorldMap.Gen(300, 300);

    vector< vector<int> > WalkMap;
    WalkMap = WorldMap.GetWalkMap();
    Path::SetMap(&WalkMap);


    //Tableau d'entité :
    vector<Entity> Entite;

    ///JEU EN LUI MEME (MAIN WHILE)

    // Create the main window
    sf::RenderWindow App(sf::VideoMode(960, 540), "Praise - v0.39.4");

    Entite.push_back(Entity(WorldMap.GetWalkTile()));
    Entite.push_back(Entity(WorldMap.GetWalkTile()));
    Entite.push_back(Entity(WorldMap.GetWalkTile()));
    Entite.push_back(Entity(WorldMap.GetWalkTile()));
    Entite.push_back(Entity(WorldMap.GetWalkTile()));

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

        for(int i = 0; i < Entite.size(); i++)
        {
            Treaded A = Entite[i].Action(WorldMap);
            if(A.PositionDepart.first == 0 && A.PositionDepart.second == 0 && A.arrivee.first == 0 && A.arrivee.second == 0)
            {

            }
            else
            {
                sf::Thread PathThread(&Path::GetPathTread, A);
                PathThread.launch();
            }

            Entite[i].draw(App);
        }

        App.display();

    }

    return 0;
}

// A faire (Propriétaire#9#): Revoir le systeme de multitreading


// A faire (Propriétaire#2#): Rendre le path finding meilleur (cout f, g, h, avoir le cjhemin vraimeent le plus court.

//i = y /// j = x
//prec.first = y /// prec.second = x
