#include "rectanglewarehouse.h"

using namespace std;

RectangleWarehouse::RectangleWarehouse(double warehouseWidth, double warehouseHeight, double hallwayWidth) :
    Warehouse(hallwayWidth),
    warehouseWidth(warehouseWidth),
    warehouseHeight(warehouseHeight)
{}

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
    vertices.clear(); // czyszczenie poprzednich wierzchołków

    double Ax = 0;
    double Ay = 0;
    vertices.push_back(Point(Ax,Ay));

    double Bx = 0;
    double By = warehouseHeight;
    vertices.push_back(Point(Bx,By));


    double Cx = warehouseWidth;
    double Cy = warehouseHeight;
    vertices.push_back(Point(Cx,Cy));


    double Dx = warehouseWidth;
    double Dy = 0;
    vertices.push_back(Point(Dx,Dy));

    for (Ware &item: wares) {
        item.y=-1;
        item.x=-1;
        item.fitted=false;
    }

    double currentX = 0;
    double currentY=0;
    int i = 0;
    int rowCount =0;


    while(1)
    {
        double rowWidth = 0, maxWareHeightInRow=0;
        int j=0;
        //checking max row height
        while(rowWidth + hallwayWidth + this->wares[i+j].getActualWidth() <= warehouseWidth)
        {
            if(i+j>=wares.size()) //checking if there is any left ware
            {
                break;
            }

            rowWidth += wares[i+j].getActualWidth();
            if(maxWareHeightInRow<wares[i+j].getActualHeight())
            {
                maxWareHeightInRow=wares[i+j].getActualHeight();
            }
            j++;
        }
        if(maxWareHeightInRow + ( rowCount%2==0? hallwayWidth : 0 ) + currentY > warehouseHeight)
        {

            currentX = wares[i-1].x + wares[i-1].getActualWidth();
            currentY = wares[i-1].y;
            while ( rowCount % 2==0 && currentX + wares[i].getActualWidth() <= warehouseWidth && currentY + wares[i].getActualHeight() <= warehouseHeight)
            {
                wares[i].x=currentX;
                wares[i].y = currentY;
                wares[i].fitted= true;
                currentX+=wares[i].getActualWidth();

                i++;
            }
            break;
        }
        for(int k =0;k<j;k++)
        {
            wares[i].x=currentX;
            wares[i].y = currentY;
            wares[i].fitted= true;

            currentX+=wares[i].getActualWidth();
            i++;
        }
        rowCount++;

        currentY+= maxWareHeightInRow;
        if(rowCount%2==1) currentY +=  hallwayWidth;

        currentX=0;
    }
}

