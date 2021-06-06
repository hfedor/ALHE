#include "rectanglewarehouse.h"

#include <list>

using namespace std;

RectangleWarehouse::RectangleWarehouse(double warehouseWidth, double warehouseHeight, double hallwayWidth) :
    Warehouse(hallwayWidth),
    warehouseWidth(warehouseWidth),
    warehouseHeight(warehouseHeight)
{
    // lewy, górny wierzchołek
    double Ax = 0;
    double Ay = 0;
    vertices.push_back(Point(Ax,Ay));

    // prawy, górny wierzchołek
    double Bx = warehouseWidth;
    double By = 0;
    vertices.push_back(Point(Bx,By));

    // prawy, dolny wierzchołek
    double Cx = warehouseWidth;
    double Cy = warehouseHeight;
    vertices.push_back(Point(Cx,Cy));

    // lewy, dolny wierzchołek
    double Dx = 0;
    double Dy = warehouseHeight;
    vertices.push_back(Point(Dx,Dy));
}

bool RectangleWarehouse::compare(RectangleWarehouse tw1, RectangleWarehouse tw2)
{
    return tw1.waresArea > tw2.waresArea;
}

/*
 *krzyżowanie wyjaśnione we wstępnym
 * w skrócie z pierwszego wybieramy geny od indexu a do b
 * a z drugiego te które nie wystąpiły w pierwszym
 */
RectangleWarehouse RectangleWarehouse::crossover(RectangleWarehouse secondParent)
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
    RectangleWarehouse child(warehouseWidth, warehouseHeight, hallwayWidth);

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

//TODO customowe wyvieranie wejścia i układ magazynu
void RectangleWarehouse::fillWithWares()
{
    // początkowo wszystkie produkty sa poza magazynem
   for (Ware &item: wares) {
       item.y=-1;
       item.x=-1;
       item.fitted=false;
   }

   double currentX = hallwayWidth;    // aktualna pozycja x'owa układania towarów w magazynie - początkowo długość korytarza
   double currentY = 0;     // aktualna pozycja y'owa układania towarów w magazynie - początkowo 0

   int rowCount = -1;   // liczba ułożonych rzędów

   double maxWareHeightInRow; // wysokość najwyższego towaru ustawionego w aktualnym rzędzie

   list<int> available_wares;    // towary jeszcze nie ułożone

   double maxRowWidth = warehouseWidth - hallwayWidth; // maksymalna długość aktulalnego rzędu towarów
   double min_ware_width = maxRowWidth; // szerokość najwęższego z towarów
   double min_ware_height = warehouseHeight;   // wyskość najniższego z towarów

   bool is_some_available = true; // czy zostały jakieś wolne towary - chyba do usunięcia i zastąpienia !available_wares.empty()

   // na początku wszystkie towary są dostępne
   for(int j = 0; j < wares.size(); j++)
   {
       wares[j].fitted = false;
       available_wares.push_back(j);

       if(min_ware_width > wares[j].getActualWidth())
           min_ware_width = wares[j].getActualWidth();

       if(min_ware_height> wares[j].getActualHeight())
           min_ware_height = wares[j].getActualHeight();
   }

   int first_ware_in_row = -1; // indeks pierwszego towaru z aktualnego rzędu

   hallwayVertices.clear();
   hallwayVertices.push_back(Point(0,0));
   hallwayVertices.push_back(Point(hallwayWidth,0));

   while(is_some_available && !available_wares.empty()) // póki są dostępne jeszcze jakiś nieułożone towary
   {
       rowCount++;
       is_some_available = false;
       bool is_first_ware_in_row_set = false;

       maxWareHeightInRow = min_ware_height; // wyskość naajwyższego towaru w rzędzie

       double row_width = 0; // aktualna szerokość rzędu

       currentX = hallwayWidth;

       list<int>::iterator j = available_wares.begin();
       while(j != available_wares.end())
       {
           double current_ware_height = wares[*j].getActualHeight();
           double current_ware_width = wares[*j].getActualWidth();

           // jeśli rzaden przedmiot już się nie zmieści
           if( row_width + min_ware_width > maxRowWidth ||
               currentY + min_ware_height + (rowCount%2==0 ? hallwayWidth : 0) > warehouseHeight)
               break;

           // jeśli produkt się nie mieści pomijamy go
           if(row_width + current_ware_width > maxRowWidth ||
                   currentY + current_ware_height + (rowCount%2==0 ? hallwayWidth : 0) > warehouseHeight)
           {
               j++;
               continue;
           }

           if(maxWareHeightInRow < current_ware_height)
               maxWareHeightInRow = current_ware_height;

           double wAx, wAy;
           wAx = currentX;
           wAy = rowCount%2==0 ?
                       currentY + maxWareHeightInRow - current_ware_height :
                       currentY;

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

       if(is_first_ware_in_row_set)
           currentY += maxWareHeightInRow;

       if(rowCount%2==0)
    {
      if(hallwayVertices.size() > 2)
          hallwayVertices.pop_back();

      double h2Ay = currentY;
      double h2Ax = hallwayWidth;
      hallwayVertices.push_back(Point(h2Ax,h2Ay));

      double h2By = h2Ay;
      double h2Bx = warehouseWidth;
      hallwayVertices.push_back(Point(h2Bx,h2By));

      currentY +=  hallwayWidth;

      if(currentY < warehouseHeight)
      {
          double h2Cy = currentY;
          double h2Cx = warehouseWidth;
          hallwayVertices.push_back(Point(h2Cx,h2Cy));

          double h2Dy = h2Cy;
          double h2Dx = hallwayWidth;
          hallwayVertices.push_back(Point(h2Dx,h2Dy));

          double h2Ey = h2Dy;
          double h2Ex = 0;
          hallwayVertices.push_back(Point(h2Ex,h2Ey));
      }
      else
      {
          double h2Ey = warehouseHeight;
          double h2Ex = warehouseWidth;
          hallwayVertices.push_back(Point(h2Ex,h2Ey));
      }
    }
   }

   hallwayVertices.push_back(Point(0, currentY));

   if(first_ware_in_row != -1 && rowCount%2==0)
   {
       currentY = wares[first_ware_in_row].y;
       currentX = wares[first_ware_in_row].x;

       maxWareHeightInRow = 0;
       maxRowWidth = hallwayWidth;
       bool last_row_added = false;

       for(list<int>::iterator j = available_wares.begin(); j != available_wares.end(); j++)
       {
           if(min_ware_width > maxRowWidth || currentY + min_ware_height > warehouseHeight)
               break;

           if(wares[*j].getActualWidth() > maxRowWidth || currentY + wares[*j].getActualHeight() > warehouseHeight)
               continue;

           if(maxWareHeightInRow < wares[*j].getActualHeight())
               maxWareHeightInRow = wares[*j].getActualHeight();

           wares[*j].x = currentX - wares[*j].getActualWidth();
           wares[*j].y = currentY;
           wares[*j].fitted = true;
           currentX = wares[*j].x;
           maxRowWidth -= wares[*j].getActualWidth();
           last_row_added = true;
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
           double h2Ex = 0;
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

bool RectangleWarehouse::isPointInside(double x,double y)
{
    return x >= 0 && x <= warehouseWidth && y >= 0 && y <= warehouseHeight;
}

double RectangleWarehouse::GetHallwayArea()
{
    if(hallwayVertices.empty())
        return 0;

    hallwayArea = 0;

    hallwayVertices.unique();

    Point last = hallwayVertices.back();
    if(0 != last.GetX())
        hallwayVertices.push_back(Point(0, warehouseHeight));

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
            hallwayArea += hallwayWidth * A.GetY();
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
            return -1;

        Point D = *i;

        if(D == hallwayVertices.back())
        {
            A.SetX(0);

            hallwayArea += hallwayWidth * A.GetY();
        }

        if(D.GetY() == C.GetY())
            hallwayArea += (B.GetX() - A.GetX() + C.GetX() - D.GetX())*(D.GetY() - A.GetY())/2;

        i++;
    }

    return hallwayArea;
}

double RectangleWarehouse::GetWarehouseArea()
{
    return warehouseWidth * warehouseHeight;
}

