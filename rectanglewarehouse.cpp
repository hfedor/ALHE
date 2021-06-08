#include "rectanglewarehouse.h"

using namespace std;

/*
 *  Konstruktor obiektów reprezentujących magazynyu w kształtach prostokątów.
 *  Konstruktor przyjmuje wartości:
 *      - warehouseWidth -  szerokość magazynu,
 *      - warehouseHeight - wysokość magazynu,
 *      - hallwayWidth -    minimalna szerokość korytarza.
 *  Ponadto konstruktor wyznacza wierzchołki magazynu i dodaje je do tablicy.
*/

RectangleWarehouse::RectangleWarehouse(double warehouseWidth, double warehouseHeight, double hallwayWidth) :
    Warehouse(hallwayWidth),
    warehouseWidth(warehouseWidth),
    warehouseHeight(warehouseHeight)
{
    // lewy, dolny wierzchołek
    double Ax = 0;
    double Ay = 0;
    vertices.push_back(Point(Ax,Ay)); // dodanie do tablicy na wierzchołki magazynu

    // prawy, dolny wierzchołek
    double Bx = warehouseWidth;
    double By = 0;
    vertices.push_back(Point(Bx,By)); // dodanie do tablicy na wierzchołki magazynu

    // prawy, górny wierzchołek
    double Cx = warehouseWidth;
    double Cy = warehouseHeight;
    vertices.push_back(Point(Cx,Cy)); // dodanie do tablicy na wierzchołki magazynu

    // lewy, górny wierzchołek
    double Dx = 0;
    double Dy = warehouseHeight;
    vertices.push_back(Point(Dx,Dy)); // dodanie do tablicy na wierzchołki magazynu
}

/*
 *  Porównanie dwóch magazynóW pod względem powierzchni zajętej przez towary.
 *  Zwraca wartość 'true', jeśli pierwszy zadany magazyn ma większą powierzchnię towarów.
 */
bool RectangleWarehouse::compare(RectangleWarehouse tw1, RectangleWarehouse tw2)
{
    return tw1.waresArea > tw2.waresArea;
}

/*
 *  Krzyżowanie wyjaśnione we wstępnym
 *  w skrócie z pierwszego wybieramy geny od indexu a do b
 *  a z drugiego te które nie wystąpiły w pierwszym
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

/*
 *  Algorytm wypełniania magazynu zadanymi towarami.
 */
void RectangleWarehouse::fillWithWares()
{
    /*
     * Początkowo wszystkie towary sa poza magazynem.
     * Towar, który jest poza magazynem ma współrzędne (-1,-1).
     */
    for (Ware &item: wares) {
        item.y = -1;
        item.x = -1;
        item.fitted = false;
    }

    /*
     *  Punkt, w którym aktualnie próbujemy wstawić towar - jego lewy, dolny wierzchołek.
     *  Początkowo punkt ten ustawiamy przy podstawie magazynu w odległości od ściany równej szerokość korytarza.
     */
    Point curr(hallwayWidth, 0);

    int rowCount = -1;   // indeks ułożonych rzędóW - początkowo -1

    /*
     * Dla każdego układanego rzędu towarów, wyliczane będą:
     *  - maxWareHeightInRow - wysokość najwyższego towaru w rzędzię,
     *  - maxRowWidth - maksymalna szerokość rzędu towaróW - maksymalna suma szerokości towaróW ustawionych w rzędzie.
     */
    double maxWareHeightInRow; // zostanie początkowo ustawiona na wysokość najmniejszego z dostępnych towarów
    double maxRowWidth = warehouseWidth - hallwayWidth; // początkwoo równa szerkośći podstawy magazynu, minus szrokość korytarza

    double min_ware_width = warehouseWidth;     // szerokość najwęższego z towarów - początkwoo zmaksymalizowana
    double min_ware_height = warehouseHeight;   // wyskość   najniższego z towarów - początkwoo zmaksymalizowana

    list<int> available_wares;    // lista towarów jeszcze nie ułożonych w magazynie

   /*
    *   Początkowo wszystkie towary są poza magazynem.
    *   W pętli wyznaczamy również minimalne wymiary dostępnych towarów.
    */
   for(auto w : wares)
   {
       available_wares.push_back(w.id);

       if(min_ware_width > w.getActualWidth())
           min_ware_width = w.getActualWidth();

       if(min_ware_height> w.getActualHeight())
           min_ware_height = w.getActualHeight();
   }

   bool is_some_available = !wares.empty(); // czy zostały jakieś wolne towary

   int first_ware_in_row = -1; // indeks pierwszego towaru z aktualnie ustawianego rzędu - początkowo -1

   /*
    *   Wyznaczanie koształtu korytarza.
    *   Początkowo wstawiamy:
    *       - lewy, górny wierzchołek magazynu i
    *       - punkt leżący na podstawie magazynu w odległości równej szerokości korytarza.
    *   Korytarz biegnie wzdłuż lewego boku magazynu.
    */
   hallwayVertices.clear();
   hallwayVertices.push_back(Point(0,0)); // lewy, dolny wierzchołke magazynu.
   hallwayVertices.push_back(Point(hallwayWidth,0));

   /*
    *   Pętla układająca kolejne rzędy towarów, do momentu, gdy nowy rząd się nie zmieści,
    *   lub skończą się towary do układania.
    */
   while(is_some_available && !available_wares.empty()) // póki są dostępne jeszcze jakiś nieułożone towary
   {
       rowCount++; // inkrementacja indeksu aktualnie układanego rzędu

       /*
        *   Na początku ustawiamy zmienną 'is_some_available' na 'false'.
        *   Jeśli w obiekguu pętli zostanie zmieniona na true, to udało się dodać jakiś towar do rzędu.
        *   Jeśli na koniec obiegu wciąż będzie równa 'false', to oznacza, że nie da się wstawić więcej towaróW.
        */
       is_some_available = false;

       maxWareHeightInRow = min_ware_height; // wyskość naajwyższego towaru w rzędzie - początkowo najmniejsza z możliwych

       double row_width = 0; // aktualna szerokość rzędu - suma szrokości towarów ułożonych w rzędzie

       /*
        *   Na początku każdego obiegu pętli zaczynamy ustawianie towarów w odległości od ściany równej
        *   szerokości korytarza.
        */
       curr.SetX(hallwayWidth);

       /*
        *   Pętla po wszystkich nieułożonych obiektach,
        *   wykonująca algorytm ukłądania towarów w rzędzie.
        */
        list<int>::iterator j = available_wares.begin();
        while(j != available_wares.end())
        {
            /*
            * Sprawdzamy, czy zmieści się jeszcze towar o najmniejszych dostępnych rozmiarach.
            * Jeśli nie - kończymy układanie rzędu.
            */
            if( row_width + min_ware_width > maxRowWidth ||
                curr.GetY() + min_ware_height + (rowCount%2==0 ? hallwayWidth : 0) > warehouseHeight)
                    break;

            /*
            *   Wczytanie wymarów aktualnie sprawdzanego towaru.
            */
            double current_ware_height = wares[*j].getActualHeight();
            double current_ware_width = wares[*j].getActualWidth();

            // jeśli produkt się nie mieści pomijamy go
            if(row_width + current_ware_width > maxRowWidth ||
                curr.GetY() + current_ware_height + (rowCount%2==0 ? hallwayWidth : 0) > warehouseHeight)
            {
                j++;
                continue;
            }

            // aktualizacja największej wysokości towau w rzędzie
            if(maxWareHeightInRow < current_ware_height)
               maxWareHeightInRow = current_ware_height;

            /*
            *   Wstawianie towaru do magazynu.
            *   Rzędy o parzystych indeksach mają nad sobą korytarz.
            *   Towary są podsuwane do korytarza, czyli
            *   w rzędach parzystych   do góry, a
            *   w rzędach nieprzystych do dołu.
            */
            wares[*j].x = curr.GetX();
            wares[*j].y = rowCount%2==0 ?
                curr.GetY() + maxWareHeightInRow - current_ware_height :
                curr.GetY();
            wares[*j].fitted = true;

            // aktualizacja szerokość rzędu
            row_width += current_ware_width;

            // aktualizacja aktualnego punktu w usawiania towarów
            curr.AddToX(current_ware_width);

            // jeśli nie ułożono wcześniej rzadnego towaru w tym rzędzie
            if(!is_some_available)
            {
                is_some_available = true;
                first_ware_in_row = *j; // aktualizacja indeksu pierwszego toawru w rzędzie
            }

            // usuwanie towaru z listy nieustawionych
            j = available_wares.erase(j);
            if(j == available_wares.end())
                break;
        }

        /*
         *   Jeśli udało sie sprawdzić ułożyć jakiś towar w rzędzie,
         *   to przesuwamy w górę punkt, w którym będziemy ustawiać towary o wysokość ułożonego rzędu.
         */
        if(is_some_available)
           curr.AddToY(maxWareHeightInRow);

        /*
         *  Jeśli indeks rzędu jest parzysty i udało się ustawić jakiś towar w rzędzie,
         *  to po ułożeniu rzędu ustawiamy nad nim korytzarz boczny.
         */
        if(rowCount%2==0 && is_some_available)
        {
            if(curr.GetY() + hallwayWidth <= warehouseHeight)
            {
                hallwayVertices.push_back(Point(hallwayWidth,   curr.GetY()));
                hallwayVertices.push_back(Point(warehouseWidth, curr.GetY()));

                curr.AddToY(hallwayWidth);

                hallwayVertices.push_back(Point(warehouseWidth, curr.GetY()));
                hallwayVertices.push_back(Point(hallwayWidth,   curr.GetY()));
            }
            else
            {
                if(hallwayVertices.size() == 2)
                    hallwayVertices.push_back(Point(hallwayWidth, warehouseHeight));

                hallwayVertices.push_back(Point(0, warehouseHeight));
            }
        }
    }

    hallwayVertices.push_back(Point(0, curr.GetY()));

    if(first_ware_in_row != -1 && rowCount%2==0)
    {
        curr.SetY(wares[first_ware_in_row].y);
        curr.SetX(wares[first_ware_in_row].x);

        maxWareHeightInRow = 0;
        maxRowWidth = hallwayWidth;
        bool last_row_added = false;

        for(list<int>::iterator j = available_wares.begin(); j != available_wares.end(); j++)
        {
            if(min_ware_width > maxRowWidth || curr.GetY() + min_ware_height > warehouseHeight)
                break;

            if(wares[*j].getActualWidth() > maxRowWidth || curr.GetY() + wares[*j].getActualHeight() > warehouseHeight)
                continue;

            if(maxWareHeightInRow < wares[*j].getActualHeight())
                maxWareHeightInRow = wares[*j].getActualHeight();

            wares[*j].x = curr.GetX() - wares[*j].getActualWidth();
            wares[*j].y = curr.GetY();
            wares[*j].fitted = true;
            curr.SetX(wares[*j].x);
            maxRowWidth -= wares[*j].getActualWidth();
            last_row_added = true;
        }

        if(last_row_added)
        {
            list<Point>::iterator i = hallwayVertices.begin();
            while(i != hallwayVertices.end())
                if(i->GetY() > curr.GetY())
                    i = hallwayVertices.erase(i);
                else
                    i++;

            double h2Ey = curr.GetY();
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

/*
 *  Obliczanie pola powierzchni magazynu.
 */
double RectangleWarehouse::GetWarehouseArea()
{
    return warehouseWidth * warehouseHeight;
}

