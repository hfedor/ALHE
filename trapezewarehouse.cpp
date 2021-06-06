#include "trapezewarehouse.h"

#include <list>

using namespace std;

TrapezeWarehouse::TrapezeWarehouse(double leftLegA, double rightLegA, double rightLegB, double hallwayWidth, double warehouseHeight) :
    Warehouse(hallwayWidth),
    leftLegA(leftLegA),
    rightLegA(rightLegA),
    rightLegB(rightLegB),
    warehouseHeight(warehouseHeight)
{
    // punkt przecięcia podstawy i lewego boku
    double Ax = 0;
    double Ay = 0;
    vertices.push_back(Point(Ax,Ay));
    hallwayVertices.push_back(Point(Ax,Ay));
    hallwayVertices.push_back(Point(Ax + hallwayWidth,Ay));

    // punkt przecięcia podstawy i prawego boku
    double Cx = - rightLegB / rightLegA;
    double Cy = 0;
    vertices.push_back(Point(Cx,Cy));

    // punkt przecięcia lewego i prawego boku
    double Ex = rightLegB / ( leftLegA - rightLegA ) ;
    double Ey = leftLegA * Ex;
    if(Ey <= warehouseHeight)
       vertices.push_back(Point(Ex,Ey));
    else
    {
        // punkt przecięcia prawego boku i górnej podstawy
        double Dx = (warehouseHeight -rightLegB) /  rightLegA ;
        double Dy = warehouseHeight;
        vertices.push_back(Point(Dx,Dy));

        // punkt przecięcia lewego boku i górnej podstawy
        double Bx = warehouseHeight /  leftLegA ;
        double By = warehouseHeight;
        vertices.push_back(Point(Bx,By));
    }
}

bool TrapezeWarehouse::compare(TrapezeWarehouse tw1, TrapezeWarehouse tw2)
{
    return tw1.waresArea > tw2.waresArea;
}

/*
 *krzyżowanie wyjaśnione we wstępnym
 * w skrócie z pierwszego wybieramy geny od indexu a do b
 * a z drugiego te które nie wystąpiły w pierwszym
 */
TrapezeWarehouse TrapezeWarehouse::crossover(TrapezeWarehouse secondParent)
{
    int firstIndex = rand()%this->wares.size();
    int lastIndex = firstIndex + rand()%(this->wares.size() - firstIndex);

    std::vector<Ware> waresFromFirstParent;
    std::vector<Ware> waresFromSecondParent;
    for(int i = firstIndex ; i<= lastIndex; i++)
        waresFromFirstParent.push_back( this->wares[i]);

    for(Ware item : secondParent.wares)
        if(!contains(waresFromFirstParent,item.id))
            waresFromSecondParent.push_back(item);

    //merging
    TrapezeWarehouse child(leftLegA, rightLegA, rightLegB, hallwayWidth, warehouseHeight);

    for(int i =0 ;i<firstIndex;i++ )
        child.wares.push_back(waresFromSecondParent[i]);

    for (auto & i : waresFromFirstParent)
        child.wares.push_back(i);

    for(int i = firstIndex;i<waresFromSecondParent.size();i++ )
        child.wares.push_back(waresFromSecondParent[i]);

    for (Ware &item: child.wares)
    {
        item.y=-1;
        item.x=-1;
        item.fitted=false;
    }

    return child;
}

void TrapezeWarehouse::fillWithWares()
{
    if(rightLegA >=0) return;

    // początkowo wszystkie produkty sa poza magazynem
    for (Ware &item: wares) {
        item.y=-1;
        item.x=-1;
        item.fitted=false;
    }

    double currentX = hallwayWidth;    // aktualna pozycja x'owa układania towarów w magazynie - początkowo długość korytarza
    double currentY= 0;     // aktualna pozycja y'owa układania towarów w magazynie - początkowo 0

    int rowCount = -1;   // liczba ułożonych rzędów

    double maxWareHeightInRow; // wysokość najwyższego towaru ustawionego w aktualnym rzędzie

    list<int> available_wares;    // towary jeszcze nie ułożone

    double maxRowWidth = (currentY - rightLegB)/rightLegA - (currentY)/leftLegA - hallwayWidth; // maksymalna długość aktulalnego rzędu towarów
    double min_ware_width = maxRowWidth; // szerokość najwęższego z towarów
    double min_ware_height = warehouseHeight;   // wyskość najniższego z towarów

    bool is_some_available =true; // czy zostały jakieś wolne towary - chyba do usunięcia i zastąpienia !available_wares.empty()

    // na początku wszystkie towary są dostępne
    for(int j = 0; j < wares.size(); j++)
    {
        available_wares.push_back(j);

        if(min_ware_width > wares[j].getActualWidth())
            min_ware_width = wares[j].getActualWidth();

        if(min_ware_height> wares[j].getActualHeight())
            min_ware_height = wares[j].getActualHeight();
    }

    int first_ware_in_row = -1; // indeks pierwszego towaru z aktualnego rzędu

    while(is_some_available && !available_wares.empty()) // póki są dostępne jeszcze jakiś nieułożone towary
    {
        rowCount++;
        is_some_available = false;
        bool is_first_ware_in_row_set = false;

        maxWareHeightInRow = min_ware_height; // wyskość naajwyższego towaru w rzędzie

        double row_width = 0; // aktualna szerokość rzędu

        double row_begin = (currentY + min_ware_height)/leftLegA + hallwayWidth; // początek aktualnego rzędu

        list<int>::iterator j = available_wares.begin();
        while(j != available_wares.end())
        {
            double current_ware_height = wares[*j].getActualHeight();
            double current_ware_width = wares[*j].getActualWidth();

            if(!is_first_ware_in_row_set)
            {
                currentX = (currentY + current_ware_height)/leftLegA + hallwayWidth; // początek rzędu, jeśli uda się postawić aktualny towar
                row_begin = currentX;
            }

            double max_row_width_for_current_ware; // maksymalna szerokość rzędu dla aktualnie sprawdzanego towaru
            double max_row_height_for_current_ware; // wyszkość rzędu do sprawdzenia dla aktualnego towaru

            max_row_height_for_current_ware =
                    rowCount%2==0 ? max(current_ware_height, maxWareHeightInRow) : current_ware_height;
            max_row_width_for_current_ware =
                    (currentY + max_row_height_for_current_ware - rightLegB)/rightLegA - row_begin;

            // jeśli rzaden przedmiot już się nie zmieści
            if( row_width + min_ware_width >
                    (rowCount%2==0 ? max_row_width_for_current_ware : (((currentY + min_ware_height - rightLegB)/rightLegA) - row_begin)) ||
                currentY + min_ware_height + (rowCount%2==0 ? hallwayWidth : 0) > warehouseHeight)
                break;

            // jeśli produkt się nie mieści pomijamy go
            if(row_width + current_ware_width > max_row_width_for_current_ware ||
                    currentY + current_ware_height + (rowCount%2==0 ? hallwayWidth : 0) > warehouseHeight)
            {
                j++;
                continue;
            }

            double wAx, wAy, wBx, wBy, wCx, wCy, wDx, wDy;
            wAx = currentX;
            wBx = currentX + current_ware_width;
            wCx = wBx;
            wDx = wAx;
            if(rowCount%2==0)
            {
                wAy = currentY + max_row_height_for_current_ware - current_ware_height;
                wBy = wAy;
                wCy = currentY + max_row_height_for_current_ware;
                wDy = wCy;
            }
            else
            {
                wAy = currentY;
                wBy = wAy;
                wCy = currentY + current_ware_height;
                wDy = wCy;
            }

            if (
                   isPointInside(wAx,wAy) && //lewy dolny
                   isPointInside(wBx,wBy) && //lewy dolny
                   isPointInside(wCx,wCy) &&//lewy górny
                   isPointInside(wDx,wDy)) //prawy górny
            {
                if(maxWareHeightInRow < current_ware_height)
                    maxWareHeightInRow = current_ware_height;

                wares[*j].x = wAx;
                wares[*j].y = wAy;
                wares[*j].fitted = true;

                row_width += current_ware_width;
                currentX += current_ware_width;

                is_some_available = true;

                if(!is_first_ware_in_row_set)
                {
                    is_first_ware_in_row_set = true;
                    first_ware_in_row = *j;
                }

                available_wares.erase(j);
                j = available_wares.begin();
            }
            else
                j++;
        }

        currentY += maxWareHeightInRow;
        if(rowCount%2==0)
        {
            if(hallwayVertices.size() > 2)
                hallwayVertices.pop_back();

            double h2Ay = currentY;
            double h2Ax = (h2Ay)/leftLegA +hallwayWidth;
            hallwayVertices.push_back(Point(h2Ax,h2Ay));

            double h2By = h2Ay;
            double h2Bx = (h2By - rightLegB)/rightLegA;
            hallwayVertices.push_back(Point(h2Bx,h2By));

            currentY +=  hallwayWidth;

            double h2Cy = currentY;
            double h2Cx = (h2Cy - rightLegB)/rightLegA;
            hallwayVertices.push_back(Point(h2Cx,h2Cy));

            double h2Dy = h2Cy;
            double h2Dx = (h2Dy)/leftLegA +hallwayWidth;
            hallwayVertices.push_back(Point(h2Dx,h2Dy));

            hallwayArea += ((h2Bx - h2Ax) + (h2Cx - h2Dx))*hallwayWidth/2;
            hallwayArea += hallwayWidth * (maxWareHeightInRow + hallwayWidth);

            double h2Ey = h2Dy;
            double h2Ex = (h2Ey)/leftLegA;
            hallwayVertices.push_back(Point(h2Ex,h2Ey));
        }
        else
            hallwayArea += hallwayWidth * maxWareHeightInRow;
    }

    //hallwayVertices.push_back(Point(currentY/leftLegA, currentY));

    if(first_ware_in_row != -1 && rowCount%2==0)
    {
        currentY = wares[first_ware_in_row].y;
        currentX = wares[first_ware_in_row].x;

        //sprawdzanie maksymalnej dłuości i punktu startowego na aktualnym y
        maxRowWidth = (currentY - rightLegB)/rightLegA - wares[first_ware_in_row].x;
        maxWareHeightInRow = 0;

        for(list<int>::iterator j = available_wares.begin(); j != available_wares.end(); j++)
        {
            if(min_ware_width > maxRowWidth || currentY + min_ware_height > warehouseHeight)
                break;

            if(wares[*j].getActualWidth() > maxRowWidth || currentY + wares[*j].getActualHeight() > warehouseHeight)
                continue;

            if(maxWareHeightInRow < wares[*j].getActualHeight())
            {
                maxWareHeightInRow = wares[*j].getActualHeight();
                maxRowWidth = (currentY + maxWareHeightInRow - rightLegB)/rightLegA - (currentY + maxWareHeightInRow)/leftLegA;
            }

            if (
                   isPointInside(currentX,currentY) && //lewy dolny
                   isPointInside(currentX-wares[*j].getActualWidth(),currentY) && //lewy dolny
                   isPointInside(currentX-wares[*j].getActualWidth(),currentY+wares[*j].getActualHeight()) &&//lewy górny
                   isPointInside(currentX,currentY+wares[*j].getActualHeight())) //prawy górny
            {
                wares[*j].x = currentX - wares[*j].getActualWidth();
                wares[*j].y = currentY;
                wares[*j].fitted = true;
                currentX = wares[*j].x;
            }
        }
    }
}

bool TrapezeWarehouse::isPointInside(double x,double y)
{
    return x>0 && leftLegA * x - y >= 0 &&  rightLegA * x + rightLegB - y >= 0;
}

double TrapezeWarehouse::GetWarehouseArea()
{
    // punkt przecięcia podstawy i lewego boku
    double Ax = 0;
    double Ay = 0;

    // punkt przecięcia podstawy i prawego boku
    double Bx = - rightLegB / rightLegA;
    double By = 0;

    // punkt przecięcia lewego i prawego boku
    double Ex = rightLegB / ( leftLegA - rightLegA ) ;
    double Ey = leftLegA * Ex;

    if(Ey <= warehouseHeight)
        return (Bx - Ax) * Ey / 2;

    // punkt przecięcia prawego boku i górnej podstawy
    double Cx = (warehouseHeight - rightLegB) /  rightLegA ;
    double Cy = warehouseHeight;

    // punkt przecięcia lewego boku i górnej podstawy
    double Dx = warehouseHeight /  leftLegA ;
    double Dy = warehouseHeight;

    return ((Bx - Ax) + (Cx - Dx))*warehouseHeight/2;
}
