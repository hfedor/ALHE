#include "individual.h"

#include <list>

using namespace std;

bool  Individual::compare(Individual i1, Individual i2)
{
    return (i1.waresArea > i2.waresArea);
}

void Individual::initTestSet()
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

void Individual::initTestSet2()
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
void Individual::initTestSet1x1(int n)
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

void Individual::initRandom(int n)
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
 *krzyżowanie wyjaśnione we wstępnym
 * w skrócie z pierwszego wybieramy geny od indexu a do b
 * a z drugiego te które nie wystąpiły w pierwszym
 */
Individual Individual::crossover(Individual secondParent)
{
    int firstIndex = rand()%this->wares.size();
    int lastIndex = firstIndex + rand()%(this->wares.size() - firstIndex);



    std::vector < Ware > waresFromFirstParent;
    std::vector < Ware > waresFromSecondParent;
    for(int i = firstIndex ; i<= lastIndex; i++)
    {
        waresFromFirstParent.push_back( this->wares[i]);
    }
    for(Ware item : secondParent.wares)
    {
        if(!contains(waresFromFirstParent,item.id))
        {
            waresFromSecondParent.push_back(item);
        }
    }

    //merging
    Individual child;
    for(int i =0 ;i<firstIndex;i++ )
    {
        child.wares.push_back(waresFromSecondParent[i]);
    }
    for (auto & i : waresFromFirstParent) {
        child.wares.push_back(i);
    }
    for(int i = firstIndex;i<waresFromSecondParent.size();i++ )
    {
        child.wares.push_back(waresFromSecondParent[i]);
    }

    for (Ware &item: child.wares) {
        item.y=-1;
        item.x=-1;
        item.fitted=false;


    }

    return child;


}

double Individual::calculateWaresArea()
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

void Individual::mutateBySwap()
{
    int firstIndex = rand()%wares.size(),secondIndex = rand()%wares.size();
    std::swap(wares[firstIndex],wares[secondIndex]);
}

void Individual::mutateByRotation()
{
    int index = rand()%wares.size();
    wares[index].rotate = !wares[index].rotate;

}

void Individual::shuffle()
{
    for(auto & ware : this->wares)
    {
        std::swap(ware,this->wares[rand()%this->wares.size()]);
    }
}

void Individual::fillRhombusWarehouse(double leftLegA,  double rightLegA, double rightLegB, int hallwayWidth,double warehouseHeight )
{
    if(rightLegA >=0) return;

    vertices.clear(); // czyszczenie poprzednich wierzchołków

    // punkt przecięcia podstawy i lewego boku
    double Ax = 0;
    double Ay = 0;
    vertices.push_back(Point(Ax,Ay));

    // punkt przecięcia podstawy i prawego boku
    double Cx = - rightLegB / rightLegA;
    double Cy = 0;
    vertices.push_back(Point(Cx,Cy));

    // punkt przecięcia lewego boku i górnej podstawy

    double Bx = warehouseHeight /  leftLegA ;
    double By = warehouseHeight;
    vertices.push_back(Point(Bx,By));

    // punkt przecięcia prawego boku i górnej podstawy
    double Dx = (warehouseHeight -rightLegB) /  rightLegA ;
    double Dy = warehouseHeight;
    vertices.push_back(Point(Dx,Dy));



    // początkowo wszystkie produkty sa poza magazynem
    for (Ware &item: wares) {
        item.y=-1;
        item.x=-1;
        item.fitted=false;
    }

    double currentX = hallwayWidth;    // aktualna pozycja x'owa układania towarów w magazynie - początkowo 0
    double currentY= 0;     // aktualna pozycja y'owa układania towarów w magazynie - początkowo 0

    int i=0;            //
    int rowCount =0;   // liczba ułożonych rzędów

   // double warehouseHeight = ware; // wysokość magazynu
    double maxWareHeightInRow = 0;

    while(true)
    {
        //sprawdzanie maksymalnej dłuości i punktu startowego na aktualnym y
        double maxRowWidth = (currentY - rightLegB)/rightLegA - (currentY)/leftLegA;
        int j;
        maxWareHeightInRow = 0;
        double rowWidth = 0;

        for(j = 0; i+j < wares.size() && rowWidth + hallwayWidth + wares[i+j].getActualWidth() < maxRowWidth; j++)
        {
            rowWidth += wares[i+j].getActualWidth();

            if(maxWareHeightInRow < wares[i+j].getActualHeight())
            {
                maxWareHeightInRow = wares[i+j].getActualHeight();
                maxRowWidth = (currentY + maxWareHeightInRow - rightLegB)/rightLegA - (currentY + maxWareHeightInRow)/leftLegA;
            }
        }
        j--;

        if(maxWareHeightInRow + ( rowCount%2==0? hallwayWidth : 0 ) + currentY > warehouseHeight || maxRowWidth < wares[i].width + hallwayWidth)
        {
            currentY = wares[i-1].y;
            int k=1;
            while(wares[i-k].y==currentY)
            {
                k++;
            }
            k--;
            currentX = wares[i-k].x;
            //sprawdzamy czy wszystkie rogi kolejnego towaru znajdują się w trójkącie
            //prawego dolnego nie sprawdzam bo już jest towar z tym punktem w magazynie
            while (rowCount%2==0 && currentY+wares[i].getActualHeight()<warehouseHeight
                   && isPointInsideTriangle(currentX-wares[i].getActualWidth(),currentY,leftLegA,0,rightLegA,rightLegB) //lewy dolny
                   && isPointInsideTriangle(currentX-wares[i].getActualWidth(),currentY+wares[i].getActualHeight(),leftLegA,0,rightLegA,rightLegB)//lewy górny
                   && isPointInsideTriangle(currentX,currentY+wares[i].getActualHeight(),leftLegA,0,rightLegA,rightLegB)) //prawy górny
            {
                wares[i].x=currentX-wares[i].getActualWidth();
                wares[i].y = currentY;
                wares[i].fitted= true;
                currentX = wares[i].x;

                i++;
            }
            break;
        }

        currentX = (currentY + maxWareHeightInRow) / leftLegA + hallwayWidth;
        for(;j >= 0 ; j--)
        {
            wares[i].x=currentX;
            wares[i].y = currentY;
            wares[i].fitted= true;

            currentX+=wares[i].getActualWidth();
            i++;
        }

        rowCount++;

        currentY+= maxWareHeightInRow;
        if(rowCount%2==1)
            currentY +=  hallwayWidth;
    }
}

/*
     * równanie prostej
     * y = ax+b
     *                /\
     * leftLeg ----> /  \ <---rightLeg
     *              /    \
     *             /______\
     *    leftleg => y=leftLegA * x + 0
     *      rightLeg => y=rightLegA * x + rightLegB
     */
void Individual::fillTriangleWarehouse(double leftLegA,  double rightLegA, double rightLegB, int hallwayWidth)
{
    if(rightLegA >=0) return;

    vertices.clear(); // czyszczenie poprzednich wierzchołków

    // punkt przecięcia podstawy i lewego boku
    double Ax = 0;
    double Ay = 0;
    vertices.push_back(Point(Ax,Ay));

    // punkt przecięcia lewego i prawego boku
    double Bx = rightLegB / ( leftLegA - rightLegA ) ;
    double By = leftLegA * Bx;
    vertices.push_back(Point(Bx,By));

    // punkt przecięcia podstawy i prawego boku
    double Cx = - rightLegB / rightLegA;
    double Cy = 0;
    vertices.push_back(Point(Cx,Cy));

    // początkowo wszystkie produkty sa poza magazynem
    for (Ware &item: wares) {
        item.y=-1;
        item.x=-1;
        item.fitted=false;
    }

    double currentX = hallwayWidth;    // aktualna pozycja x'owa układania towarów w magazynie - początkowo długość korytarza
    double currentY= 0;     // aktualna pozycja y'owa układania towarów w magazynie - początkowo 0

    int rowCount = -1;   // liczba ułożonych rzędów

    double warehouseHeight = By; // wysokość magazynu
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
                currentY + min_ware_height > warehouseHeight)
                break;


            // jeśli produkt się nie mieści pomijamy go
            if(row_width + current_ware_width > max_row_width_for_current_ware ||
                    currentY + current_ware_height > warehouseHeight)
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
                   isPointInsideTriangle(wAx,wAy,leftLegA,0,rightLegA,rightLegB) && //lewy dolny
                   isPointInsideTriangle(wBx,wBy,leftLegA,0,rightLegA,rightLegB) && //lewy dolny
                   isPointInsideTriangle(wCx,wCy,leftLegA,0,rightLegA,rightLegB) &&//lewy górny
                   isPointInsideTriangle(wDx,wDy,leftLegA,0,rightLegA,rightLegB)) //prawy górny
            {
                if(maxWareHeightInRow < current_ware_height)
                {
                    maxWareHeightInRow = current_ware_height;
                }
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
            currentY +=  hallwayWidth;
    }

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
                   isPointInsideTriangle(currentX,currentY,leftLegA,0,rightLegA,rightLegB) && //lewy dolny
                   isPointInsideTriangle(currentX-wares[*j].getActualWidth(),currentY,leftLegA,0,rightLegA,rightLegB) && //lewy dolny
                   isPointInsideTriangle(currentX-wares[*j].getActualWidth(),currentY+wares[*j].getActualHeight(),leftLegA,0,rightLegA,rightLegB) &&//lewy górny
                   isPointInsideTriangle(currentX,currentY+wares[*j].getActualHeight(),leftLegA,0,rightLegA,rightLegB)) //prawy górny
            {
                wares[*j].x = currentX - wares[*j].getActualWidth();
                wares[*j].y = currentY;
                wares[*j].fitted = true;
                currentX = wares[*j].x;
            }
        }
    }
}

//TODO customowe wyvieranie wejścia i układ magazynu
void Individual::fillRectangleWarehouse(int warehouseWidth, int warehouseHeight, int hallwayWidth)
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

bool Individual::contains(std::vector<Ware> vector,int id)
{
    for (Ware item : vector)
    {
        if(item.id == id) return true;
    }
    return false;
}


bool Individual::isPointInsideTriangle(double x,double y, double leftLegA, double leftLegB, double rightLegA, double rightLegB)
{

    return x>0 && leftLegA * x + leftLegB - y >=0 &&  rightLegA * x + rightLegB - y>=0;
}
