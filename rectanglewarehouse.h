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
    static bool compare(RectangleWarehouse, RectangleWarehouse);
    RectangleWarehouse crossover(RectangleWarehouse);
    void fillWithWares();
    virtual double GetWarehouseArea();
    virtual double GetHallwayArea();
private:
    bool isPointInside(double x,double y);
};

#endif // RECTANGLEWAREHOUSE_H
