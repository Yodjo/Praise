#ifndef PATH_H
#define PATH_H

#include <iostream>
#include <vector>
#include <cmath>
#include <list>
#include <map>
#include <algorithm>

#include "const.h"


class Path
{
    public:
        /** Default constructor */
        Path();
        /** Default destructor */
        virtual ~Path();

        /** Path Initialisation : */
        static void Init(std::vector<std::vector<int> > *WalkMap, std::list<Treaded> *pWaiting);

        /** Get du path */
        static void PathThread(Tinit &init);
        static std::vector<std::pair<int, int> > GetPath(const std::pair<int, int> &PositionDepart,const std::pair<int, int> &arrivee);
        static void GetPathTread(Treaded &TreadingInfo);
        static void AddPathTask(Treaded &TreadingInfo);
        static std::list<Treaded>* GetWaitingAdd();

        /** Mutex */
        static void LockMutex();
        static void UnLockMutex();

    protected:
        static int dist(const std::pair<int, int> &a,const std::pair<int, int> &b);
        static bool in_liste(std::pair<int,int> &n, std::map<std::pair<int,int>, Node>& l);
        static std::pair<int,int> meilleur_noeud(std::map<std::pair<int,int>, Node>& l);


    private:
    static std::vector< std::vector<int> > *Map;
    static std::list<Treaded> *Waiting;
    static sf::Mutex mutexPath;

};

#endif // PATH_H
