#ifndef CONST_H_INCLUDED
#define CONST_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <list>

//Objet
enum {BUCHE};

enum {W_OUI, W_EAU, W_TREE, W_NON};
enum {EAU, PLAGE, PLAINE, MONTAGNE, PIC};

//Enumeration d'action
//Action :
enum {NOTHING, CUT_TREE, BUILD_HOUSE, CULTIVER};
//Sous  action :
    //Couper arbre :
enum {TROUVE_ARBRE, ALLER_ARBRE, COUPE_ARBRE, ENTREPOSER_ARBRE};
    //Construire maison :
enum {CLEAN_ZONE, GET_RESSOURCE, BUILD_BLOCK};
    //Cultiver :
enum {RETOURNER, PLANTER, ATTENDRE, RECOLTER};

#define TAILLE_ARBRE_MAX 5
#define ACTION_TIME 200
#define NO_LAG_TIME 40

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
    int Taille = 1;
};

typedef struct Treaded Treaded;
struct Treaded
{
    Treaded(std::vector<std::pair<int, int> > *C, std::pair<int, int>D, std::pair<int, int>A, bool *Rc) : Chemin(C), PositionDepart(D), arrivee(A), Recherche(Rc)
    {Chemin = C;}

    bool operator==(Treaded const &a) const
    {
        if(a.Chemin == Chemin && a.Recherche == Recherche)
            return true;
        else
            return false;
    }

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
