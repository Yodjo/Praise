#include "../include/stock.h"

using namespace std;

Stock::Stock(int Msize) : sizeMax(Msize)
{
    //ctor
}

Stock::~Stock()
{
    //dtor
}

bool Stock::Contient(int Type)
{
    if( count(Inventaire.begin(), Inventaire.end(), Type) > 0 )
       return true;
    else
        return false;

}

bool Stock::Use(int Type)
{
    if( count(Inventaire.begin(), Inventaire.end(), Type) > 0 )
    {
        vector<int>::iterator it = find(Inventaire.begin(), Inventaire.end(), Type);
        Inventaire.erase(it);

        return true;
    }
    else
    {
        return false;
    }

}

bool Stock::UseMultiple(int Type, int Number)
{
    if( count(Inventaire.begin(), Inventaire.end(), Type) >= Number )
    {
        for(int i = 0; i < Number; ++i)
        {
            vector<int>::iterator it = find(Inventaire.begin(), Inventaire.end(), Type);
            Inventaire.erase(it);
        }

        return true;
    }
    else
    {
        return false;
    }

}

bool Stock::Add(int Type, int Nbr)
{
    for(int i = 0; i < Nbr; ++i)
    Inventaire.push_back(Type);
}

void Stock::Voir()
{
    if(!Inventaire.empty())
    {
        int nbBuche = count(Inventaire.begin(), Inventaire.end(), int(BUCHE) );

        cout << " * Inventaire * " << endl;
        cout << "- Buche : " << nbBuche <<  endl;
    }
}
