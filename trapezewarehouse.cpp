#include "trapezewarehouse.h"

#include <list>

using namespace std;

TrapezeWarehouse::TrapezeWarehouse(double leftLegA, double rightLegA, double rightLegB, double hallwayWidth, double new_warehouseHeight) :
    Warehouse(hallwayWidth),
    leftLegA(leftLegA),
    rightLegA(rightLegA),
    rightLegB(rightLegB),
    warehouseHeight(new_warehouseHeight)
{
    // punkt przecięcia podstawy i lewego boku
    double Ax = 0;
    double Ay = 0;
    vertices.push_back(Point(Ax,Ay));

    // punkt przecięcia podstawy i prawego boku
    double Cx = - rightLegB / rightLegA;
    double Cy = 0;
    vertices.push_back(Point(Cx,Cy));

    // punkt przecięcia lewego i prawego boku
    double Ex = rightLegB / ( leftLegA - rightLegA ) ;
    double Ey = leftLegA * Ex;
    if(Ey <= warehouseHeight)
    {
       vertices.push_back(Point(Ex,Ey));
       warehouseHeight = Ey;
    }
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

    hallwayX = hallwayWidth*sqrt(1 + 1/(leftLegA * leftLegA));
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

    double currentX = hallwayX;    // aktualna pozycja x'owa układania towarów w magazynie - początkowo długość korytarza
    double currentY= 0;     // aktualna pozycja y'owa układania towarów w magazynie - początkowo 0

    int rowCount = -1;   // liczba ułożonych rzędów

    double maxWareHeightInRow; // wysokość najwyższego towaru ustawionego w aktualnym rzędzie

    list<int> available_wares;    // towary jeszcze nie ułożone

    double maxRowWidth = (currentY - rightLegB)/rightLegA - (currentY)/leftLegA - hallwayX; // maksymalna długość aktulalnego rzędu towarów
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

    hallwayVertices.clear();
    hallwayVertices.push_back(Point(0,0));
    hallwayVertices.push_back(Point(hallwayX,0));

    while(is_some_available && !available_wares.empty()) // póki są dostępne jeszcze jakiś nieułożone towary
    {
        rowCount++;
        is_some_available = false;
        bool is_first_ware_in_row_set = false;

        maxWareHeightInRow = min_ware_height; // wyskość naajwyższego towaru w rzędzie

        double row_width = 0; // aktualna szerokość rzędu

        double row_begin = (currentY + min_ware_height)/leftLegA + hallwayX; // początek aktualnego rzędu
        double currentX = row_begin;

        list<int>::iterator j = available_wares.begin();
        while(j != available_wares.end())
        {
            double current_ware_height = wares[*j].getActualHeight();
            double current_ware_width = wares[*j].getActualWidth();

            if(currentX < (currentY + current_ware_height)/leftLegA + hallwayX)
                currentX = (currentY + current_ware_height)/leftLegA + hallwayX; // początek rzędu, jeśli uda się postawić aktualny towar

            if(!is_first_ware_in_row_set)
                row_begin = currentX;

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

        if(is_first_ware_in_row_set)
            currentY += maxWareHeightInRow;

        if(rowCount%2==0)
        {
            if(hallwayVertices.size() > 2)
                hallwayVertices.pop_back();

            double h2Ay = currentY;
            double h2Ax = min((h2Ay)/leftLegA + hallwayX, (h2Ay - rightLegB)/rightLegA);
            hallwayVertices.push_back(Point(h2Ax,h2Ay));

            double h2By = h2Ay;
            double h2Bx = (h2By - rightLegB)/rightLegA;
            hallwayVertices.push_back(Point(h2Bx,h2By));

            currentY +=  hallwayWidth;

            if(currentY < warehouseHeight)
            {
                double h2Cy = currentY;
                double h2Cx = (h2Cy - rightLegB)/rightLegA;
                hallwayVertices.push_back(Point(h2Cx,h2Cy));

                double h2Dy = h2Cy;
                double h2Dx = (h2Dy)/leftLegA +hallwayX;
                hallwayVertices.push_back(Point(h2Dx,h2Dy));

                double h2Ey = h2Dy;
                double h2Ex = (h2Ey)/leftLegA;
                hallwayVertices.push_back(Point(h2Ex,h2Ey));
            }
            else
            {
                double h2Ey = warehouseHeight;
                double h2Ex = (h2Ey - rightLegB)/rightLegA;
                hallwayVertices.push_back(Point(h2Ex,h2Ey));
            }
        }
    }

    hallwayVertices.push_back(Point(currentY/leftLegA, currentY));

    if(first_ware_in_row != -1 && rowCount%2==0)
    {
        currentY = wares[first_ware_in_row].y;
        currentX = wares[first_ware_in_row].x;

        //sprawdzanie maksymalnej dłuości i punktu startowego na aktualnym y
        maxRowWidth = (currentY - rightLegB)/rightLegA - wares[first_ware_in_row].x;
        maxWareHeightInRow = 0;
        bool last_row_added = false;

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

                last_row_added = true;
            }
        }

        if(last_row_added)
        {
            list<Point>::iterator i = hallwayVertices.begin();
            while(i != hallwayVertices.end())
                if(i->GetY() > currentY)
                    i = hallwayVertices.erase(i);
                else
                    i++;

            double h2Ey = currentY;
            double h2Ex = (h2Ey)/leftLegA;
            hallwayVertices.push_back(Point(h2Ex,h2Ey));
        }
    }

    list<Point>::iterator i = hallwayVertices.begin();
    while(i != hallwayVertices.end())
        if(!isPointInside(i->GetX(),i->GetY()))
            i = hallwayVertices.erase(i);
        else
            i++;
}

bool TrapezeWarehouse::isPointInside(double x,double y)
{
    return x>=0 && leftLegA * x - y >= 0 &&  rightLegA * x + rightLegB - y >= 0;
}

double TrapezeWarehouse::GetHallwayArea()
{
    if(hallwayVertices.empty())
        return 0;

    hallwayArea = 0;

    hallwayVertices.unique();

    Point last = hallwayVertices.back();
    if(last.GetY()/leftLegA != last.GetX())
        hallwayVertices.push_back(Point(warehouseHeight/leftLegA, warehouseHeight));

    list<Point>::iterator j = hallwayVertices.begin();
    for(;j != hallwayVertices.end();j++)
        cout << "(" << j->GetX() << ":" << j->GetY() << ")" << endl;

    list<Point>::iterator i = hallwayVertices.begin();
    if(i != hallwayVertices.end())
        i++;
    else
        return -1;

    if(i == hallwayVertices.end())
        return -1;

    i++;
    if(i == hallwayVertices.end())
        return -1;

    while( i != hallwayVertices.end() )
    {
        Point A = *i;

        i++;
        if(i == hallwayVertices.end())
        {
            hallwayArea += hallwayX * A.GetY();
            break;
        }

        Point B = *i;

        if(A.GetY() != B.GetY())
            return -1;

        i++;
        if(i == hallwayVertices.end())
            return -1;

        Point C = *i;

        i++;
        if(i == hallwayVertices.end())
        {
            A.SetX(A.GetY()/leftLegA);

            hallwayArea += hallwayX * A.GetY();
            hallwayArea += (B.GetX() - A.GetX())*(C.GetY() - A.GetY())/2;
            break;
        }

        Point D = *i;

        if(D == hallwayVertices.back())
        {
            A.SetX(A.GetY()/leftLegA);

            hallwayArea += hallwayX * A.GetY();
        }

        if(D.GetY() == C.GetY())
            hallwayArea += (B.GetX() - A.GetX() + C.GetX() - D.GetX())*(D.GetY() - A.GetY())/2;

        i++;
    }

    return hallwayArea;
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
