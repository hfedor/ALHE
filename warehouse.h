#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <iostream>
#include <time.h>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "Ware.h"
#include "point.h"

class Warehouse
{
protected:
    std::vector <Ware> wares; // tablica towarów
    std::vector<Point> vertices; // tablica wierzchołków
    double waresArea = 0; // powierzchnia zajmowana przez towary
    double hallwayArea = 0;
    std::vector<Point> hallwayVertices;
    double hallwayWidth; // szerokość korytarz
public:
    Warehouse(double hallwayWidth);
    void adaptationFunction();
    double calculateWaresArea();
     bool compare(Warehouse*);
    bool contains(std::vector<Ware> vector,int id);
    void initTestSet();
    void initTestSet2();
    void initTestSet1x1(int n);
    void initRandom(int n);
    std::vector<Point> GetVerticles() { return vertices; }
    std::vector<Point> GetHallwayVerticles() { return hallwayVertices; }
    std::vector<Ware> GetWares() { return wares; }
    double GetWaresArea();
    virtual double GetWarehouseArea() = 0;
    void mutateBySwap();
    void mutateByRotation();
    virtual void fillWithWares() = 0;
    void randomize();
    void setWares(std::vector<Ware> wares){this->wares = wares;}
};

#endif // WAREHOUSE_H
