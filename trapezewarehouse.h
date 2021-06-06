#ifndef TRAPEZEWAREHOUSE_H
#define TRAPEZEWAREHOUSE_H

#include <iostream>
#include <time.h>
#include <cstdlib>
#include <algorithm>

#include "warehouse.h"

class TrapezeWarehouse : public Warehouse
{
    double leftLegA;
    double rightLegA;
    double rightLegB;
    double warehouseHeight;
public:
    TrapezeWarehouse(double leftLegA, double rightLegA, double rightLegB, double hallwayWidth, double warehouseHeight);
    static bool compare(TrapezeWarehouse, TrapezeWarehouse);
    TrapezeWarehouse crossover(TrapezeWarehouse);
    void fillWithWares();
    virtual double GetWarehouseArea();
    virtual double GetHallwayArea();
private:
    bool isPointInside(double x,double y);
};

#endif // TRAPEZEWAREHOUSE_H
