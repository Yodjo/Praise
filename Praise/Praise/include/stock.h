#ifndef STOCK_H
#define STOCK_H

#include <iostream>

#include <SFML/Graphics.hpp>

#include <vector>
#include <algorithm>

#include "const.h"


class Stock
{
    public:
        /** Default constructor */
        Stock(int Msize);
        /** Default destructor */
        virtual ~Stock();

        bool Contient(int Type);
        bool Use(int Type);
        bool Add(int Type);

    protected:
    private:
        int sizeMax;
        std::vector<int> Inventaire;
};

#endif // STOCK_H
