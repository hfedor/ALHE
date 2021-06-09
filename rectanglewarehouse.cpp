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
    int firstIndex = rand()%this->wares.size(); //wybieramy index od 0 do ostatniego inedksu
    int lastIndex = firstIndex + rand()%(this->wares.size() - firstIndex); //wybiermy index większy od pierwszego i mniejszy od rozmiaru tablicy

    std::vector<Ware> waresFromFirstParent; //tablica na pobrane geny z pierwszego rodzica
    std::vector<Ware> waresFromSecondParent; //tablica na pobrane geny z drugiego rodzica
    for(int i = firstIndex ; i<= lastIndex; i++)
        waresFromFirstParent.push_back( this->wares[i]); //pobieramy z genotypu pierwszego osobnika geny od indeksu "firstIndex" do indeksu "lastIndex"

    for(Ware item : secondParent.wares)
        if(!contains(waresFromFirstParent,item.id)) // pobieramy geny które nie zostały pobrane z pierwszego osobnika zaczynając od początku tablicy
            waresFromSecondParent.push_back(item);

    //merging
    RectangleWarehouse child(warehouseWidth, warehouseHeight, hallwayWidth); //tworzymy nasze dziecko

    for(int i =0 ;i<firstIndex;i++ )
        child.wares.push_back(waresFromSecondParent[i]); //uzupełniamy nasze dziecko pierwszą częścią genów z drugiego rodzica

    for (auto & i : waresFromFirstParent)
        child.wares.push_back(i); //uzupełniamy nasze dziecko genami z pierwszego rodzica

    for(int i = firstIndex;i<waresFromSecondParent.size();i++ )
        child.wares.push_back(waresFromSecondParent[i]); //uzupełniamy nasze dziecko drugą częścią genów z drugiego rodzica

    //uzupełniamy resztę właściwości w naszym dziecku
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

    list<double> min_ware_width;    // lista na posegregowane szerokości dostępnych towarów - od najwęższych
    list<double> min_ware_height;   // lista na posegregowane wysokości  dostępnych towarów - od najniższych

    list<int> available_wares;    // lista towarów jeszcze nie ułożonych w magazynie

   /*
    *   Początkowo wszystkie towary są poza magazynem.
    *   W pętli dodajemy również wymiary towarów do odpowiednich list.
    */
   for(auto w : wares)
   {
       available_wares.push_back(w.id);

       min_ware_width.push_back(w.getActualWidth());
       min_ware_height.push_back(w.getActualHeight());
   }

   //   Trzymanie minimalnych wymiarów towarów pozwala na prostą aktualizcję tych wymiarów po usunięciu towaru z listy dostępnych towarów.
   min_ware_width.sort();
   min_ware_width.sort();

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
    *   Ustawiając rzędy możemy się spotkać z sytuacją, w której jesteśmy w stanie ustawićjeszcze jakiś towar
    *   na danej wysokości, ale jeśli za tym towarem ustawilibyśmy kolejny towar, nie byłby do niego dostępu z korytarza,
    *   ponieważ górna ściana magazynu jest zbyt nisko, aby zmieścił się jeszcze jeden korytarz poziomy.
    *   W takiej sytuacji jesteśmy wstawić jedynie jeden towar w rzędzie.
    *   Zmienna 'sealed' zawiera informacje, czy taka sytuacja miała miejsce - czy ostatni rząd został zapombowany przez
    *   jeden towar.
    */
   bool sealed = false;

   /*
    *   Pętla układająca kolejne rzędy towarów, do momentu, gdy nowy rząd się nie zmieści,
    *   lub skończą się towary do układania.
    */
   while(is_some_available && !available_wares.empty())
   {
       rowCount++; // inkrementacja indeksu aktualnie układanego rzędu

       /*
        *   Na początku ustawiamy zmienną 'is_some_available' na 'false'.
        *   Jeśli w obiekguu pętli zostanie zmieniona na true, to udało się dodać jakiś towar do rzędu.
        *   Jeśli na koniec obiegu wciąż będzie równa 'false', to oznacza, że nie da się wstawić więcej towaróW.
        */
       is_some_available = false;

       maxWareHeightInRow = min_ware_height.front(); // wyskość naajwyższego towaru w rzędzie - początkowo najmniejsza z możliwych

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
            if( row_width + min_ware_width.front() > maxRowWidth)
                break;

            if( curr.GetY() + min_ware_height.front() + (rowCount%2==0 ? hallwayWidth : 0) > warehouseHeight)
            {
                if(is_some_available || curr.GetY() + min_ware_height.front() > warehouseHeight)
                    break;
            }

            //   Wczytanie wymarów aktualnie sprawdzanego towaru.
            double current_ware_height = wares[*j].getActualHeight();
            double current_ware_width = wares[*j].getActualWidth();

            // jeśli produkt się nie mieści na szerokość pomijamy go
            if(row_width + current_ware_width > maxRowWidth)
            {
                j++;
                continue;
            }

            // jeśli produkt się nie mieści na wysokość pomijamy go
            if(curr.GetY() + current_ware_height + (rowCount%2==0 ? hallwayWidth : 0) > warehouseHeight)
            {
                if(rowCount%2==1 || is_some_available || curr.GetY() + current_ware_height > warehouseHeight)
                {
                    j++;
                    continue;
                }
                else
                    sealed = true;
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

            if(sealed)
                break;

            // aktualizacja minimalnych wymiarów dostępnych towarów
            if(current_ware_width == min_ware_width.front())
                min_ware_width.pop_front();
            if(current_ware_height == min_ware_height.front())
                min_ware_height.pop_front();

            // usuwanie towaru z listy nieustawionych
            j = available_wares.erase(j);
            if(j == available_wares.end())
                break;
        }

        /*
         *   Jeśli udało sie ułożyć jakiś towar w rzędzie,
         *   to przesuwamy w górę punkt, w którym będziemy ustawiać towary o wysokość ułożonego rzędu.
         */
        if(is_some_available)
           curr.AddToY(maxWareHeightInRow);

        /*
         *  Jeśli indeks rzędu jest parzysty i udało się ustawić jakiś towar w rzędzie,
         *  to po ułożeniu rzędu ustawiamy nad nim korytarz boczny.
         */
        if(rowCount%2==0 && is_some_available && !sealed)
        {
            //  Jeśli na aktualnej wysokości zmieści się jeszcze korytarz
            if(curr.GetY() + hallwayWidth <= warehouseHeight)
            {
                /*  Dodawanie prostokątnego korytarza biegnącego wzdłuż rzędu towarów
                 *  biegnącego od korytarza przy lewej ściany magazynu do prawej ściany.
                 */
                hallwayVertices.push_back(Point(hallwayWidth,   curr.GetY()));
                hallwayVertices.push_back(Point(warehouseWidth, curr.GetY()));

                curr.AddToY(hallwayWidth);

                hallwayVertices.push_back(Point(warehouseWidth, curr.GetY()));
                hallwayVertices.push_back(Point(hallwayWidth,   curr.GetY()));
            }
            else // Jeśli nie zmieści się kolejny korytarz pozimy (z uwagi na górną ścianę magazynu)
            {
                hallwayVertices.push_back(Point(hallwayWidth, warehouseHeight));
                hallwayVertices.push_back(Point(0, warehouseHeight));
            }
        }
    }

    /*
     *  Jeśli udało się ułożyc nieparzystą liczbę rzędów (więkkszą niż jeden), to znaczy, że korytarz
     *  za ostatnim rzędem nie jest potrzebny i można ustawić towary między już ustawionym rzędem,
     *  a lewą ścianą magazynu.
     */
    if(first_ware_in_row != -1 && rowCount%2==0 && rowCount > 0)
    {
        //  Ustawienie aktualnej poycji na pierwsdzy towar z ostatneigo rzędu.
        curr.SetY(wares[first_ware_in_row].y);
        curr.SetX(wares[first_ware_in_row].x);

        //  Ustawienie askymalnej szerokości do ustawienia przez produkty na szerokość korytarza
        maxRowWidth = hallwayWidth;

        // Pętla po wszystkich dostępnych towarach realizujuąca algorytm uzupełniania ostatniego rzędu.
        for(list<int>::iterator j = available_wares.begin(); j != available_wares.end(); j++)
        {
            /*
            * Sprawdzamy, czy zmieści się jeszcze towar o najmniejszych dostępnych rozmiarach.
            * Jeśli nie - kończymy układanie rzędu.
            */
            if(min_ware_width.front() > maxRowWidth || curr.GetY() + min_ware_height.front() > warehouseHeight)
                break;

            //   Wczytanie wymarów aktualnie sprawdzanego towaru.
            double current_ware_height = wares[*j].getActualHeight();
            double current_ware_width = wares[*j].getActualWidth();

            // jeśli produkt się nie mieści pomijamy go
            if(current_ware_width > maxRowWidth || curr.GetY() + current_ware_height > warehouseHeight)
                continue;

            //   Wstawianie towaru do magazynu.
            wares[*j].x = curr.GetX() - current_ware_width;
            wares[*j].y = curr.GetY();
            wares[*j].fitted = true;

            //  Aktualizacja współrzędnej x'owej początku rzędu.
            curr.SetX(wares[*j].x);

            //  Aktualizacja dostępnej szerokości do uzupełnmienia towarami.
            maxRowWidth -= current_ware_width;

            // aktualizacja minimalnych wymiarów dostępnych towarów
            if(current_ware_width == min_ware_width.front())
                min_ware_width.pop_front();
            if(current_ware_height == min_ware_height.front())
                min_ware_height.pop_front();

            // usuwanie towaru z listy nieustawionych
            j = available_wares.erase(j);
            if(j == available_wares.end())
                break;

        }
    }

    // zkończenie korytarza
    hallwayVertices.push_back(Point(hallwayWidth, curr.GetY()));
    hallwayVertices.push_back(Point(0, curr.GetY()));
}

/*
 *  Obliczanie pola powierzchni korytarza.
 *  W przypadku błędu zwraca wartość -1.
 */
double RectangleWarehouse::GetHallwayArea()
{
    //  Jeśli nie podano żadnego wierzchołka korytarza pole jest równe 0.
    if(hallwayVertices.empty())
        return 0;

    //  Ustawienie początkowego pola na wartość 0.
    hallwayArea = 0;

    //  Pozbycie się powtórzonych wierzchołków korytarza.
    hallwayVertices.unique();

    //  Pominięcie dwóch pierwszych wierzchołków.
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

    // Pężtla po pozostałych wierzchołkach korytarza obliczająca jego powierzchnię.
    while( i != hallwayVertices.end() )
    {
        //  lewy, dolny wirzchołke poziomego korytarza, lub ostatni wierzchołke korytarza
        Point A = *i;

        i++;
        //  Jeśli A jest ostatnim wierzchołkiem korytarza obliczamy powierzchnię korytarza pionowego i kończymy obliczanie.
        if(i == hallwayVertices.end())
        {
            hallwayArea += hallwayWidth * A.GetY();
            return hallwayArea;
        }

        //  prawy, dolny wierzchołkem poziomego korytarza
        Point B = *i;

        //  Jeśli wierzchołki A i B nie są na róWnym poziomie zwraca błąd.
        if(A.GetY() != B.GetY())
            return -1;

        i++;
        if(i == hallwayVertices.end())
            return -1;

        //  lewy, górny wierzchołkem poziomego korytarza
        Point C = *i;

        i++;
        if(i == hallwayVertices.end())
            return -1;

        //  prawy, górny wierzchołkem poziomego korytarza
        Point D = *i;


        //  Jeśli wierzchołki C i D są na róWnym poziomie oblicza powierzchnię korytarza poziomego,
        if(D.GetY() == C.GetY())
            hallwayArea += (B.GetX() - A.GetX() + C.GetX() - D.GetX())*(D.GetY() - A.GetY())/2;
        else // a jeśli nie, to zwraca błąD.
            return -1;

        i++;
    }

    // Niepoprawna liczba wierzchołków.
    return -1;
}

/*
 *  Obliczanie pola powierzchni magazynu.
 */
double RectangleWarehouse::GetWarehouseArea()
{
    return warehouseWidth * warehouseHeight;
}

