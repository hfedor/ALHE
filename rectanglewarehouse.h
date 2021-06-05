#ifndef RECTANGLEWAREHOUSE_H
#define RECTANGLEWAREHOUSE_H

#include "warehouse.h"

#include <iostream>
#include <time.h>
#include <cstdlib>
#include <algorithm>
#include <vector>

class RectangleWarehouse : public Warehouse
{
    double warehouseWidth; // szerokość magazynu
    double warehouseHeight; // wysokość magazynu
public:
    RectangleWarehouse(double warehouseWidth, double warehouseHeight, double hallwayWidth);
    bool compare(RectangleWarehouse);
    RectangleWarehouse crossover(RectangleWarehouse);
    void fillWithWares();
private:
};

#endif // RECTANGLEWAREHOUSE_H
