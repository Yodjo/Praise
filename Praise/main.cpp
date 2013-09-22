#include <iostream>
#include <vector>
#include <list>

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

    list<Treaded> Wt;
    Path::Init(&WalkMap, &Wt);

    //Tableau d'entité :
    vector<Entity> Entite;

    ///JEU EN LUI MEME (MAIN WHILE)

    // Create the main window
    sf::RenderWindow App(sf::VideoMode(960, 540), "Praise - v0.39.4");
    sf::View vue = App.getDefaultView();
    vue.setCenter(0, 0);
    App.setView(vue);

    sf::Thread PathThreading(&Path::PathThread, Tinit(&App, Path::GetWaitingAdd()) );
    PathThreading.launch();

    Entite.push_back(Entity(WorldMap.GetWalkTile()));
    Entite.push_back(Entity(WorldMap.GetWalkTile()));
    Entite.push_back(Entity(WorldMap.GetWalkTile()));
    Entite.push_back(Entity(WorldMap.GetWalkTile()));
    Entite.push_back(Entity(WorldMap.GetWalkTile()));


    //Boucle d'action des entity.
    int i = 0;
    sf::Clock EntWhile;

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

            if(event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up)
                {
                    vue.move(0, -1);
                }
                if (event.key.code == sf::Keyboard::Down)
                {
                    vue.move(0, 1);
                }
                if (event.key.code == sf::Keyboard::Left)
                {
                    vue.move(-1, 0);
                }
                if (event.key.code == sf::Keyboard::Right)
                {
                    vue.move(1, 0);
                }

                if (event.key.code == sf::Keyboard::Add)
                {
                    vue.zoom(0.5f);
                }
                if (event.key.code == sf::Keyboard::Subtract)
                {
                    vue.zoom(1.5f);
                }
            }


            App.setView(vue);
        }

        App.clear();


        WorldMap.affMiniMap(App);


        EntWhile.restart();
        while(i < Entite.size() && EntWhile.getElapsedTime() < sf::milliseconds(NO_LAG_TIME))
        {
            Treaded A = Entite[i].Action(WorldMap);

            if(A.Chemin == NULL /*|| (A.arrivee.first == 0 && A.arrivee.second == 0) || (A.PositionDepart.first == 0 && A.PositionDepart.second == 0)*/)
            {
                //cout << __FUNCTION__ << "- line " << __LINE__ << endl;
            }
            else //if(A.Chemin != NULL)
            {
                Path::AddPathTask(A);
            }
            i++;
        }
        if(i >= Entite.size())
        i = 0;

        for(int y = 0; y < Entite.size(); y++)
        Entite[y].draw(App);

        App.display();

    }

    return 0;
}

// A faire (Propriétaire#9#): Reajouter le systeme de multitreading


// A faire (Propriétaire#2#): Rendre le path finding meilleur (cout f, g, h, avoir le cjhemin vraimeent le plus court.

//i = y /// j = x
//prec.first = y /// prec.second = x
