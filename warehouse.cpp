#include "warehouse.h"

using namespace std;

Warehouse::Warehouse(double hallwayWidth) :
    hallwayWidth(hallwayWidth)
{}

void Warehouse::adaptationFunction()
{
    fillWithWares();
    calculateWaresArea();
}

bool Warehouse::compare(Warehouse* warehouse)
{
    return warehouse->waresArea > waresArea;
}

double Warehouse::calculateWaresArea()
{
    int i = 0;
    double sum = 0;
    while(wares[i].fitted)
    {
        sum += wares[i].getArea();
        i++;
    }
    this->waresArea= sum;
    return waresArea;
}

bool Warehouse::contains(std::vector<Ware> vector,int id)
{
    for (Ware item : vector)
        if(item.id == id) return true;

    return false;
}

void Warehouse::initTestSet()
{                   //0,1,2,3,4,5,6,7,8,9,10,11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39
    int tabWidth[] = {4,5,5,5,4,5,6,1,9,2, 6, 2, 8, 1, 6, 9, 9, 4, 1, 9, 4, 5, 5, 5, 4, 5, 6, 1, 9, 2, 6, 2, 8, 1, 6, 9, 9, 4, 1, 9};
    int tabHight[] = {9,4,4,2,8,1,7,3,2,7, 7, 3, 3, 8, 4, 9, 5, 9, 1, 8, 9, 4, 4, 2, 8, 1, 7, 3, 2, 7, 7, 3, 3, 8, 4, 9, 5, 9, 1, 8};
    for (int i = 0; i < 40; ++i)
    {
        Ware ware;
        ware.id = i;
        ware.width = tabWidth[i];
        ware.height =tabHight[i];
        wares.push_back(ware);
    }
}

void Warehouse::initTestSet2()
{
    int tabWidth[] = {9,9,2,4,9,5,4,6,8,4,2,4,3,5,4,4,5,5,5,4,5,6,1,9,2,6,2,8,1,6,9,9,4,1,9};
    int tabHight[] = {9,9,7,8,8,9,9,7,9,9,9,9,8,9,9,9,4,4,2,8,1,7,3,2,7,7,3,3,8,4,9,5,9,1,8};
    for (int i = 0; i < 35; ++i)
    {
        Ware ware;
        ware.id = i;
        ware.width = tabWidth[i],
        ware.height =tabHight[i];
        wares.push_back(ware);
    }
}

void Warehouse::initTestSet1x1(int n)
{

    for (int i = 0; i < n; ++i)
    {
        Ware ware;
        ware.id = i;
        ware.width = 1;
        ware.height =1;
        wares.push_back(ware);
    }
}

void Warehouse::initRandom(int n)
{
    srand(time(NULL));
    for (int i = 0; i < n; ++i)
    {
        Ware ware;
        ware.id = i;
        ware.width = 1 + rand()%4;
        ware.height = 1 + rand()%4;
        wares.push_back(ware);
    }
}

void Warehouse::mutateBySwap()
{
    int firstIndex = rand()%wares.size(),secondIndex = rand()%wares.size();
    std::swap(wares[firstIndex],wares[secondIndex]);
}

void Warehouse::mutateByRotation()
{
    int index = rand()%wares.size();
    wares[index].rotate = !wares[index].rotate;

}
