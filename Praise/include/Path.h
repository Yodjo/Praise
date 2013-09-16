#ifndef PATH_H
#define PATH_H

#include <iostream>
#include <vector>
#include <cmath>
#include <list>
#include <map>

#include "const.h"


class Path
{
    public:
        /** Default constructor */
        Path();
        /** Default destructor */
        virtual ~Path();

        /** Path Initialisation : */
        static void SetMap(std::vector<std::vector<int> > *WalkMap);

        /** Get du path */
        static std::vector<std::pair<int, int> > GetPath(std::pair<int, int> PositionDepart, std::pair<int, int> arrivee);
        static void GetPathTread(Treaded &TreadingInfo);

    protected:
        static int dist(std::pair<int, int> a, std::pair<int, int> b);
        static bool in_liste(std::pair<int,int> n, std::map<std::pair<int,int>, Node>& l);
        static std::pair<int,int> meilleur_noeud(std::map<std::pair<int,int>, Node>& l);


    private:
    static std::vector< std::vector<int> > *Map;
};

#endif // PATH_H
