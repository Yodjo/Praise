#ifndef CONST_H_INCLUDED
#define CONST_H_INCLUDED

typedef struct Treaded Treaded;
struct Treaded
{
    Treaded(std::vector<std::pair<int, int> > *C, std::pair<int, int>D, std::pair<int, int>A) : Chemin(C), PositionDepart(D), arrivee(A)
    {Chemin = C;}

    std::vector<std::pair<int, int> > *Chemin;
    std::pair<int, int> PositionDepart;
    std::pair<int, int> arrivee;
};

typedef struct Node Node;
struct Node
{
    int Qualite;
    std::pair<int, int> Parent;
};

#endif // CONST_H_INCLUDED
