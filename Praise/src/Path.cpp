#include "../include/Path.h"

using namespace std;


vector< vector<int> > *Path::Map;

Path::Path()
{
    //ctor
}

Path::~Path()
{
    //dtor
}

/// FONCTIONS D'INITIALISATION :

void Path::SetMap(std::vector<std::vector<int> > *WalkMap)
{
    Map = WalkMap;
}

/// PATH FINDING
int Path::dist(pair<int, int> a, pair<int, int> b)
{
     return sqrt((a.first - b.first)*(a.first - b.first) + (a.second - b.second)* (a.second - b.second));
}

bool Path::in_liste(pair<int,int> n, map<pair<int,int>, Node>& l)
{
    map<pair<int,int>, Node>::iterator i = l.find(n);
    if (i==l.end())
        return false;
    else
        return true;
}

pair<int,int> Path::meilleur_noeud(map<pair<int,int>, Node>& l)
{
    float di = l.begin()->second.Qualite;
    pair<int,int> m_noeud = l.begin()->first;

    for (map<pair<int,int>, Node>::iterator i = l.begin(); i!=l.end(); i++)
        if (i->second.Qualite < di)
        {
            di = i->second.Qualite;
            m_noeud = i->first;
        }

    return m_noeud;
}

vector<pair<int, int> > Path::GetPath(pair<int, int> PositionDepart, pair<int, int> arrivee)
{
        //A*
    map<pair<int, int>, Node> Ouverte;
    map<pair<int, int>, Node> Fermee;

    pair<int, int> current;
    current.first = PositionDepart.first;
    current.second = PositionDepart.second;

    Node Depart;
    Depart.Parent.first = current.first;
    Depart.Parent.second = current.second;
    Depart.Qualite = dist(current, arrivee);

    Ouverte[current] = Depart;

    /// Boucle tant que l'on a pas de solution ou que l'on sait qu'il n'y a pas de solution
    while(current != arrivee && !Ouverte.empty())
    {
        /// On prend le noeud avec la meilleur qualité
        current = meilleur_noeud(Ouverte);

        /// On l'ajoute a la liste fermée
        Fermee[current] = Ouverte[current];

        /// On le suprrime de la liste
        Ouverte.erase(current);

        /// On teste toutes les case autour
        for(int i = current.first-1; i <= current.first+1; i++)
        {
            //Si la case se trouve en dehors des limite on ne la teste pas.
            if(i < 0 || i > Map->size()-1)
                continue;

            for(int j = current.second-1; j <= current.second+1; j++)
            {

                //Si la case se trouve en dehors des limite on ne la teste pas.
                if(j < 0 || j > Map[i].size()-1)
                    continue;

                //Si la case est la case actuellement en test
                if(current.first == i && current.second == j)
                    continue;

                //Si la case est un obstacle on passe
                if( Map->at(i)[j] == 1)
                    continue;

                    ///On a donc une case valide
                    pair<int, int> cur(i, j); //current

                    ///Si elle ne fait pas partie de liste fermée
                    if(!in_liste(cur, Fermee))
                    {
                        ///On l'ajoute
                        Node Tmp;
                        Tmp.Qualite = dist(cur, arrivee);
                        Tmp.Parent = current;

                        ///Si elle fait déjà partie de la liste ouverte on la met à jour
                        if(in_liste(cur, Ouverte))
                        {
                            //Si la qualité est meilleur:
                            if(Fermee[current].Qualite >= Fermee[Ouverte[cur].Parent].Qualite)
                            Ouverte[cur].Parent = Tmp.Parent;
                        }
                        else ///Si elle ne s'y trouve pas on l'ajoute
                        {
                            Ouverte[cur] = Tmp;
                        }
                    }

            }
        }
    }

    if(Ouverte.empty())
    {
        vector<pair<int, int> >Chemin;
        return Chemin;
    }
    else
    {
        vector<pair<int, int> >Chemin;
        Node &Path = Fermee[arrivee];

        pair<int, int> prec = arrivee;
        Chemin.insert(Chemin.begin(), prec);

        while(prec != Depart.Parent)
        {
            prec = Path.Parent;
            Path = Fermee[prec];
            Chemin.insert(Chemin.begin(), prec);
        }

        return Chemin;
    }
}

void Path::GetPathTread(Treaded &TreadingInfo)
{
    *(TreadingInfo.Chemin) = GetPath(TreadingInfo.PositionDepart, TreadingInfo.arrivee);
}
