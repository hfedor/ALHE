#ifndef TRIANGLEWAREHOUSE_H
#define TRIANGLEWAREHOUSE_H

#include <iostream>
#include <time.h>
#include <cstdlib>
#include <algorithm>
#include "warehouse.h"

#include <vector>

class TriangleWarehouse : public Warehouse
{
    double leftLegA;
    double rightLegA;
    double rightLegB;
public:
    TriangleWarehouse(double leftLegA, double rightLegA, double rightLegB, double hallwayWidth);
    static bool compare(TriangleWarehouse, TriangleWarehouse);
    TriangleWarehouse crossover(TriangleWarehouse);
    void fillWithWares();
private:
    bool isPointInside(double x,double y, double leftLegA, double leftLegB, double rightLegA, double rightLegB);
};

#endif // TRIANGLEWAREHOUSE_H
