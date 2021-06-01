//
// Created by Michal on 18.05.2021.
//

#ifndef UNTITLED3_INDIVIDUAL_H
#define UNTITLED3_INDIVIDUAL_H

#include <vector>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include<time.h>

#include "point.h"
#include "Ware.h"


class Individual {
private:
    std::vector < Ware > wares;
    std::vector<Point> vertices;
    double waresArea=0;

    bool contains(std::vector<Ware> vector,int id);
    bool isPointInsideTriangle(double x,double y, double leftLegA, double leftLegB, double rightLegA, double rightLegB);
public:
    std::vector<Point> GetVerticles() { return vertices; }
    std::vector<Ware> GetWares() { return wares; }
    static bool  compare(Individual i1, Individual i2);
    void initTestSet();
    void initTestSet2();
    void initTestSet1x1(int n);
    void initRandom(int n);
    void mutateBySwap();
    void mutateByRotation();
    double calculateWaresArea();
    Individual crossover(Individual secondParent);
    void shuffle();
    void fillRectangleWarehouse(int warehouseWidth, int warehouseHeight, int hallwayWidth);
    void fillTriangleWarehouse(double leftLegA,  double rightLegA, double rightLegB, int hallwayWidth);
    void fillRhombusWarehouse(double leftLegA,  double rightLegA, double rightLegB, int hallwayWidth,double warehouseHeight );
};


#endif //UNTITLED3_INDIVIDUAL_H
