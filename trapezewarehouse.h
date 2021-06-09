#ifndef TRAPEZEWAREHOUSE_H
#define TRAPEZEWAREHOUSE_H

#include "warehouse.h"

/*
 *  klasa obiektów repezentujących magazyny w kształtach trapezów
*/
class TrapezeWarehouse : public Warehouse
{
    double leftLegA;    // współczynnik A postej zawierającej lewey bok trapeza
    double rightLegA;   // współczynnik A postej zawierającej prawy bok trapeza
    double rightLegB;   // współczynnik B postej zawierającej prawy bok trapeza
    double warehouseHeight; // wysokość trapeza
    double hallwayX;    // szerokość korytarza na prostej x
public:
    TrapezeWarehouse(double leftLegA, double rightLegA, double rightLegB, double hallwayWidth, double warehouseHeight);
    static bool compare(TrapezeWarehouse, TrapezeWarehouse);
    TrapezeWarehouse crossover(TrapezeWarehouse);
    virtual double GetWarehouseArea();
    virtual double GetHallwayArea();
private:
    void fillWithWares();
    bool isPointInside(double x,double y);
};

#endif // TRAPEZEWAREHOUSE_H
