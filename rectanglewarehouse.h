#ifndef RECTANGLEWAREHOUSE_H
#define RECTANGLEWAREHOUSE_H

#include "warehouse.h"

/*
 *  klasa obiektów repezentujących magazyny w kształtach prostokątów
*/
class RectangleWarehouse : public Warehouse
{
    double warehouseWidth; // szerokość magazynu
    double warehouseHeight; // wysokość magazynu
public:
    RectangleWarehouse(double warehouseWidth, double warehouseHeight, double hallwayWidth);
    static bool compare(RectangleWarehouse, RectangleWarehouse);
    RectangleWarehouse crossover(RectangleWarehouse);
    virtual double GetWarehouseArea();
    virtual double GetHallwayArea();
private:
    virtual void fillWithWares();
};

#endif // RECTANGLEWAREHOUSE_H
