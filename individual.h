//
// Created by Michal on 18.05.2021.
//

#ifndef UNTITLED3_INDIVIDUAL_H
#define UNTITLED3_INDIVIDUAL_H

#include <vector>
#include<iostream>
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
    bool isPointInsideTriangle(int x,int y, double leftLegA, double leftLegB, double rightLegA, double rightLegB);
public:
    std::vector<Point> GetVerticles() { return vertices; }
    std::vector<Ware> GetWares() { return wares; }
    double getWaresArea(){return waresArea;}
    void setWares(std::vector<Ware> wares){this->wares = wares;}
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

    double calcActualMaxWidth(double leftLegA, double rightLegA, double rightLegB, int hallwayWidth, int currentY,
                                  int maxWareHeightInRow) const {
            double left =/* floor*/((currentY + maxWareHeightInRow - rightLegB) / rightLegA);
            double right =/*ceil*/((currentY + maxWareHeightInRow ) / leftLegA );
            double actualMaxWidth = left - right -  hallwayWidth;
            return actualMaxWidth;
        }
};


#endif //UNTITLED3_INDIVIDUAL_H
