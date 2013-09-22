#ifndef CONST_H_INCLUDED
#define CONST_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <list>

enum {W_OUI, W_EAU, W_TREE, W_NON};
enum {EAU, PLAGE, PLAINE, MONTAGNE, PIC};

#define TAILLE_ARBRE_MAX 5
#define ACTION_TIME 200
#define NO_LAG_TIME 200

//Info
#define VIEW_SIZE 10

enum {SAPIN, POMMIER};

typedef struct Arbre Arbre;
struct Arbre
{
    Arbre(int mType = -1) : Type(mType)
    {

    }

    int Type;
    int Taille;
};

typedef struct Treaded Treaded;
struct Treaded
{
    Treaded(std::vector<std::pair<int, int> > *C, std::pair<int, int>D, std::pair<int, int>A, bool *Rc) : Chemin(C), PositionDepart(D), arrivee(A), Recherche(Rc)
    {Chemin = C;}

    std::vector<std::pair<int, int> > *Chemin;
    std::pair<int, int> PositionDepart;
    std::pair<int, int> arrivee;
    bool *Recherche;
};

typedef struct Node Node;
struct Node
{
    int Qualite;
    std::pair<int, int> Parent;
};

typedef struct CivInfo CivInfo;
struct CivInfo
{
    std::string Nom;
    sf::Color Couleur;
};

typedef struct Tinit Tinit;
struct Tinit
{
    Tinit(sf::RenderWindow *App, std::list< Treaded > *Waiting) : pApp(App), pWaiting(Waiting)
    {

    }

    sf::RenderWindow *pApp;
    std::list< Treaded > *pWaiting;
};

#endif // CONST_H_INCLUDED
