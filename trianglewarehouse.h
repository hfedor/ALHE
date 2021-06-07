#ifndef TRIANGLEWAREHOUSE_H
#define TRIANGLEWAREHOUSE_H

#include "warehouse.h"

/*
 *  klasa obiektów repezentujących magazyny w kształtach trójkątów
*/
class TriangleWarehouse : public Warehouse
{
    double leftLegA;    // współczynnik A postej zawierającej lewey bok tójkąta
    double rightLegA;   // współczynnik A postej zawierającej prawy bok tójkąta
    double rightLegB;   // współczynnik B postej zawierającej prawy bok tójkąta
    double hallwayX;    // szerokość korytarza na prostej x
public:
    TriangleWarehouse(double leftLegA, double rightLegA, double rightLegB, double hallwayWidth);
    static bool compare(TriangleWarehouse, TriangleWarehouse);
    TriangleWarehouse crossover(TriangleWarehouse);
    virtual double GetWarehouseArea();
    virtual double GetHallwayArea();
private:
    void fillWithWares();
    bool isPointInside(double x,double y);
};

#endif // TRIANGLEWAREHOUSE_H
