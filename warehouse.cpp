#include "warehouse.h"

using namespace std;

/*
 *  Konstruktor wirtualnej klasy obiektów reprezentujących magazyny.
 *  Konstruktor przyjmuje wartość - hallwayWidth - minimalna szerokość korytarza.
 */
Warehouse::Warehouse(double hallwayWidth) :
    hallwayWidth(hallwayWidth)
{}

/*
 *  Funkcja adaptacji wykorzystywana w algorytmie ewolucyjnym.
 */
void Warehouse::adaptationFunction()
{
    fillWithWares();
    calculateWaresArea();
}

/*
 *  Obliczenie powierzchni magazynu zajmowanej przez towary.
 */
double Warehouse::calculateWaresArea()
{
    //  Wyzerowanie powierzchni magazynu.
    waresArea = 0;

    //  Pętla po wszystkich towarach dodająca powierzchnię towarów ustawionych w magazynie do symy powierzchni.
    for(vector<Ware>::iterator w = wares.begin(); w != wares.end(); w++)
        if((*w).fitted)
            waresArea += (*w).getArea();

    return waresArea;
}

/*
 *  Sprawdzenie, czy istnieje towar o zadanym indeksie.
 */
bool Warehouse::contains(std::vector<Ware> vector,int id)
{
    for (Ware item : vector)
        if(item.id == id) return true;

    return false;
}

/*
 *  Generowanie zadanej liczby towarów o losowanych wymiarach.
 */
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

/*
 *  Mutacja przez zamianę losowanych towarów miejscami
 */
void Warehouse::mutateBySwap()
{
    int firstIndex = rand()%wares.size(), secondIndex = rand()%wares.size();
    std::swap(wares[firstIndex],wares[secondIndex]);
}

/*
 *  Mutacja przez rotację losowanego towaru.
 */
void Warehouse::mutateByRotation()
{
    int index = rand()%wares.size();
    wares[index].rotate = !wares[index].rotate;

}

/*
 *  Obliczenie powierzchni magazynu zajmowanej przez towary.
 */
double Warehouse::GetWaresArea()
{
    return calculateWaresArea();
}

/*
 *  Wymieszanie towarów kolejnością na liście i losowa rotacja towaróW.
 */
void Warehouse::randomize()
{
    for(vector<Ware>::iterator w = wares.begin(); w != wares.end(); w++)
    {
        std::swap(*w,wares[rand()%wares.size()]);
        if(rand()%2 == 0)
            w->rotate = !w->rotate;
    }
}
