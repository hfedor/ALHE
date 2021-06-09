#include "trapezewarehouse.h"

using namespace std;

/*
 *  Konstruktor obiektów reprezentujących magazynyu w kształtach trapezów.
 *  Trapez opisany jest przez dwie proste, zwierające jego ramiona i dwie podstawy.
 *  Równania prostych zawierające ramiona trapezu:
 *      - y = leftLegA * x + 0          - prosta zawierająca lewe ramię o początku w punkcie (0,0).
 *      - y = rightLegA * x + rightLegB - prosta zawierająca prawe ramię
 *  Konstruktor przyjmuje wartości:
 *      - leftLegA
 *      - rightLegA
 *      - rightLegB
 *      - new_warehouseHeight - wysokość magazynu - jeśli jest zbyt duża, magazyn ma kształ trójkąta,
 *      - hallwayWidth -    minimalna szerokość korytarza.
 *  Ponadto konstruktor wyznacza wierzchołki magazynu i dodaje je do tablicy.
*/
TrapezeWarehouse::TrapezeWarehouse(double leftLegA, double rightLegA, double rightLegB, double hallwayWidth, double new_warehouseHeight) :
    Warehouse(hallwayWidth),
    leftLegA(leftLegA),
    rightLegA(rightLegA),
    rightLegB(rightLegB),
    warehouseHeight(new_warehouseHeight)
{
    // punkt przecięcia dolnej podstawy i lewego boku
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
    // Jeśli pinkt E znajduje się poniżej zadanej wysokości magazynu magazyn przyjmuje kształ trójkąta.
    if(Ey <= warehouseHeight)
    {
       vertices.push_back(Point(Ex,Ey));
       warehouseHeight = Ey;
    }
    else // W przeciwnym przypadku magazyn ma kształ trapezu.
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

    // Obliczenie odległości między lewą ściana magazynu, a końcem korytarza
    hallwayX = hallwayWidth*sqrt(1 + 1/(leftLegA * leftLegA));
}

/*
 *  Porównanie dwóch magazynóW pod względem powierzchni zajętej przez towary.
 *  Zwraca wartość 'true', jeśli pierwszy zadany magazyn ma większą powierzchnię towarów.
 */
bool TrapezeWarehouse::compare(TrapezeWarehouse tw1, TrapezeWarehouse tw2)
{
    return tw1.waresArea > tw2.waresArea;
}

/*
 *  Krzyżowanie wyjaśnione we wstępnym projekcie
 *  Z pierwszego osobnika wybieramy geny od indexu a do b
 *  a z drugiego te które nie wystąpiły w pierwszym
 */
TrapezeWarehouse TrapezeWarehouse::crossover(TrapezeWarehouse secondParent)
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
    TrapezeWarehouse child(leftLegA, rightLegA, rightLegB, hallwayWidth, warehouseHeight); //tworzymy nasze dziecko

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
void TrapezeWarehouse::fillWithWares()
{
    if(rightLegA >=0) return;

    /*
     * Początkowo wszystkie towary sa poza magazynem.
     * Towar, który jest poza magazynem ma współrzędne (-1,-1).
     */
    for (Ware &item: wares) {
        item.y=-1;
        item.x=-1;
        item.fitted=false;
    }

    /*
     *  Punkt, w którym aktualnie próbujemy wstawić towar - jego lewy, dolny wierzchołek.
     *  Początkowo punkt ten ustawiamy przy podstawie magazynu w odległości od ściany równej szerokość korytarza.
     */
    Point curr(hallwayX, 0);

    int rowCount = -1;   // indeks ułożonych rzędóW - początkowo -1

    /*
     * Dla każdego układanego rzędu towarów, wyliczane będą:
     *  - maxWareHeightInRow - wysokość najwyższego towaru w rzędzię,
     *  - maxRowWidth - maksymalna szerokość rzędu towaróW - maksymalna suma szerokości towaróW ustawionych w rzędzie.
     */
    double maxWareHeightInRow; // zostanie początkowo ustawiona na wysokość najmniejszego z dostępnych towarów
    double maxRowWidth;

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

    bool is_some_available = !wares.empty(); // czy zostały jakieś wolne towary i jednocześnie czy ustawiono już jakiś towar w rzędzie

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
    hallwayVertices.push_back(Point(hallwayX,0)); // lewy, dolny wierzchołke magazynu.

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

        double row_width = 0; // aktualna szerokość rzędu - suma szrokości towarów ułożonych w rzędzie

        maxWareHeightInRow = min_ware_height.front(); // wyskość naajwyższego towaru w rzędzie

        // początek aktualnie układanego rzędu - początkowo ???
        double row_begin = (curr.GetY() + min_ware_height.front())/leftLegA + hallwayX;

        curr.SetX(row_begin);

        /*
         *   Pętla po wszystkich nieułożonych obiektach,
         *   wykonująca algorytm ukłądania towarów w rzędzie.
         */
        list<int>::iterator j = available_wares.begin();
        while(j != available_wares.end())
        {
            //   Wczytanie wymarów aktualnie sprawdzanego towaru.
            double current_ware_height = wares[*j].getActualHeight();
            double current_ware_width = wares[*j].getActualWidth();

            /*
             *  Ponieważ towar musi być odsunięty od lewej ścainy o określoną szerokość, wynikającą z szerokości korytarza
             *  to jego współrzędna x'owa zależo od jego wysokości - lewy górny wierzchołek musi być oddalony od ściany
             *  na określoną szerokość.
             */
            if(curr.GetX() < (curr.GetY() + current_ware_height)/leftLegA + hallwayX)
                curr.SetX((curr.GetY() + current_ware_height)/leftLegA + hallwayX); // początek rzędu, jeśli uda się postawić aktualny towar

            // Jeśli akualnie ustawiamy pierwszy towar w rzędzie, to wartośc zawierająca początek rzędui jest aktualizowana.
            if(!is_some_available)
                row_begin = curr.GetX();

            double max_row_width_for_current_ware; // maksymalna szerokość rzędu dla aktualnie sprawdzanego towaru
            double max_row_height_for_current_ware; // wyszkość rzędu do sprawdzenia dla aktualnego towaru

            /*
             *  Jeśli aktualnie ustawiany rząd ma parzysty indeks, to oznacza to, że ponad tym rzędem musi znajdować się korytarz.
             *  W takim przypadku, towary ustawiane są w rzędzie w taki sposób, że sa one podsunęte do góry do korytarza.
             *  Miejsce na nowe towary, które można ustawić jeszcze w tym rzędzie zależą od szerokości obszaru między
             *  prawym, górnym wierzchołkiem ostatnio ustawionego towaru, a ścianą.
             *  Magazyn przyjmuje tylko wartości 'rightLegA' < 0, więc prawe ramie magazynu zwęża się do środka.
             *  Oznacza to, że chociaż między ostatnim towarem, a ścianą może znajdować się obszar, w którym udało by się
             *  wstawić jeszcze jakieś towary, po podsunięciu ich do dołu, to nie można tego zrobić, poniewaź poprzedni towar
             *  blokowałby do nich dostęp z korytarza.
             *
             *  W przypadku rzędów o nieparzystych indeksach są styczne do poziomego krytarza od dołu, więc są one przysuwane
             *  w dół przy ustawianiu ich w rzędzie. Przy układaniu takich rzędów nie występuje problem blokowania dostępu
             *  do towarów przez wcześniej ułożony towar. Szerokośc obszaru dostępnego do układania nowych towarów zależy od
             *  wysokości aktualnie ustawianego towaru, a nie wyskości najwyższego towaru w rzędzie.
             */
            max_row_height_for_current_ware =
                    rowCount%2==0 ? max(current_ware_height, maxWareHeightInRow) : current_ware_height;
            max_row_width_for_current_ware =
                    (curr.GetY() + max_row_height_for_current_ware - rightLegB)/rightLegA - row_begin;

            /*
            * Sprawdzamy, czy zmieści się jeszcze towar o najmniejszych dostępnych rozmiarach.
            * Jeśli nie - kończymy układanie rzędu.
            */
            if( row_width + min_ware_width.front() >
                    (rowCount%2==0 ? max_row_width_for_current_ware : (((curr.GetY() + min_ware_height.front() - rightLegB)/rightLegA) - row_begin)))
                break;

            if( curr.GetY() + min_ware_height.front() + (rowCount%2==0 ? hallwayWidth : 0) > warehouseHeight )
            {
                if(is_some_available || curr.GetY() + min_ware_height.front() > warehouseHeight)
                    break;
            }

            // jeśli produkt się nie mieści na szerokość pomijamy go
            if(row_width + current_ware_width > max_row_width_for_current_ware )
            {
                j++;
                continue;
            }

            // jeśli produkt się nie mieści na wysokość pomijamy go
            if( curr.GetY() + current_ware_height + (rowCount%2==0 ? hallwayWidth : 0) > warehouseHeight)
            {
                if(rowCount%2==1 || is_some_available || curr.GetY() + current_ware_height > warehouseHeight)
                {
                    j++;
                    continue;
                }
                else
                    sealed = true;
            }

            //  wyznaczanie pozycji wierzchołków towaru
            double wAx, wAy, wBx, wBy, wCx, wCy, wDx, wDy;
            wAx = curr.GetX();
            wBx = curr.GetX() + current_ware_width;
            wCx = wBx;
            wDx = wAx;
            if(rowCount%2==0) // Jeśli indeks rzędu jest parzysty podsuwamy towar do góry.
            {
                wAy = curr.GetY() + max_row_height_for_current_ware - current_ware_height;
                wBy = wAy;
                wCy = curr.GetY() + max_row_height_for_current_ware;
                wDy = wCy;
            }
            else // Jeśli indeks rzędu jest nieparzysty podsuwamy towar do dołu.
            {
                wAy = curr.GetY();
                wBy = wAy;
                wCy = curr.GetY() + current_ware_height;
                wDy = wCy;
            }

            // Sprawdzamy, czy produkt zmieści się w magazynie.
            if (
                   isPointInside(wAx,wAy) && //lewy dolny
                   isPointInside(wBx,wBy) && //lewy dolny
                   isPointInside(wCx,wCy) &&//lewy górny
                   isPointInside(wDx,wDy)) //prawy górny
            {
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
                wares[*j].x = wAx;
                wares[*j].y = wAy;
                wares[*j].fitted = true;

                // aktualizacja szerokość rzędu
                row_width += current_ware_width;

                // aktualizacja aktualnego punktu w usawiania towarów
                curr.AddToX(current_ware_width);

                // jeśli nie ułożono wcześniej rzadnego towaru w tym rzędzie
                if(!is_some_available)
                {
                    is_some_available = true;
                    first_ware_in_row = *j;
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
                double h2Ay = curr.GetY();
                double h2Ax = min((h2Ay)/leftLegA + hallwayX, (h2Ay - rightLegB)/rightLegA);
                hallwayVertices.push_back(Point(h2Ax,h2Ay));

                double h2By = h2Ay;
                double h2Bx = (h2By - rightLegB)/rightLegA;
                hallwayVertices.push_back(Point(h2Bx,h2By));

                curr.AddToY(hallwayWidth);

                if(curr.GetY() < warehouseHeight)
                {
                    double h2Cy = curr.GetY();
                    double h2Cx = (h2Cy - rightLegB)/rightLegA;
                    hallwayVertices.push_back(Point(h2Cx,h2Cy));

                    if((h2Cy)/leftLegA + hallwayX < h2Cx)
                    {
                        double h2Dy = h2Cy;
                        double h2Dx = (h2Dy)/leftLegA + hallwayX;
                        hallwayVertices.push_back(Point(h2Dx,h2Dy));
                    }
                }
                else
                {
                    double h2Ey = warehouseHeight;
                    double h2Ex = (h2Ey - rightLegB)/rightLegA;
                    hallwayVertices.push_back(Point(h2Ex,h2Ey));
                }
            }
            else // Jeśli nie zmieści się kolejny korytarz pozimy (z uwagi na górną ścianę magazynu)
            {
                hallwayVertices.push_back(Point(hallwayX, warehouseHeight));
                hallwayVertices.push_back(Point(warehouseHeight/leftLegA, warehouseHeight));
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

        //sprawdzanie maksymalnej dłuości i punktu startowego na aktualnym y
        maxRowWidth = (curr.GetY() - rightLegB)/rightLegA - wares[first_ware_in_row].x;

        for(list<int>::iterator j = available_wares.begin(); j != available_wares.end(); j++)
        {
            /*
            * Sprawdzamy, czy zmieści się jeszcze towar o najmniejszych dostępnych rozmiarach.
            * Jeśli nie - kończymy układanie rzędu.
            */
            if(min_ware_width.front() > maxRowWidth || curr.GetY() + min_ware_height.front() > warehouseHeight)
                break;

            //   Wczytanie wymarów aktualnie sprawdzanego towaru.
            if(wares[*j].getActualWidth() > maxRowWidth || curr.GetY() + wares[*j].getActualHeight() > warehouseHeight)
                continue;

            //   Wczytanie wymarów aktualnie sprawdzanego towaru.
            double current_ware_height = wares[*j].getActualHeight();
            double current_ware_width = wares[*j].getActualWidth();

            if (
                   isPointInside(curr.GetX(),curr.GetY()) && //lewy dolny
                   isPointInside(curr.GetX() - current_ware_width, curr.GetY()) && //lewy dolny
                   isPointInside(curr.GetX() - current_ware_width, curr.GetY() + current_ware_height) &&//lewy górny
                   isPointInside(curr.GetX(), curr.GetY() + current_ware_height)) //prawy górny
            {

                /*
                *   Wstawianie towaru do magazynu.
                *   Rzędy o parzystych indeksach mają nad sobą korytarz.
                *   Towary są podsuwane do korytarza, czyli
                *   w rzędach parzystych   do góry, a
                *   w rzędach nieprzystych do dołu.
                */
                wares[*j].x = curr.GetX() - current_ware_width;
                wares[*j].y = curr.GetY();
                wares[*j].fitted = true;

                //  Aktualizacja współrzędnej x'owej początku rzędu.
                curr.SetX(wares[*j].x);

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
    }

    // zkończenie korytarza
    hallwayVertices.push_back(Point(min(curr.GetY()/leftLegA + hallwayX, (curr.GetY() - rightLegB)/rightLegA), curr.GetY()));
    hallwayVertices.push_back(Point(curr.GetY()/leftLegA, curr.GetY()));
}

/*
 *  Sprawdzenie, czy punkt o zadanych współrzędnych znajduje się w magazynie.
 */
bool TrapezeWarehouse::isPointInside(double x,double y)
{
    return x>=0 && leftLegA * x - y >= 0 &&  rightLegA * x + rightLegB - y >= 0;
}

/*
 *  Obliczanie pola powierzchni korytarza.
 *  W przypadku błędu zwraca wartość -1.
 */
double TrapezeWarehouse::GetHallwayArea()
{
    //  Jeśli nie podano żadnego wierzchołka korytarza pole jest równe 0.
    if(hallwayVertices.empty())
        return 0;

    //  Ustawienie początkowego pola na wartość 0.
    hallwayArea = 0;

    //  Pozbycie się powtórzonych wierzchołków korytarza.
    hallwayVertices.unique();

    Point last = hallwayVertices.back();
    if(last.GetY()/leftLegA != last.GetX())
        hallwayVertices.push_back(Point(warehouseHeight/leftLegA, warehouseHeight));

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
            hallwayArea += hallwayX * A.GetY();
            break;
        }

        Point B = *i;

        //  prawy, dolny wierzchołkem poziomego korytarza
        if(A.GetY() != B.GetY())
            return -1;

        i++;
        if(i == hallwayVertices.end())
            return -1;

        //  lewy, górny wierzchołkem poziomego korytarza
        Point C = *i;

        i++;
        if(i == hallwayVertices.end())
        {
            A.SetX(A.GetY()/leftLegA);

            hallwayArea += hallwayX * A.GetY();
            hallwayArea += (B.GetX() - A.GetX())*(C.GetY() - A.GetY())/2;
            break;
        }

        //  prawy, górny wierzchołkem poziomego korytarza
        Point D = *i;

        if(D == hallwayVertices.back())
        {
            A.SetX(A.GetY()/leftLegA);

            hallwayArea += hallwayX * A.GetY();
        }

        //  Jeśli wierzchołki C i D są na róWnym poziomie oblicza powierzchnię korytarza poziomego,
        if(D.GetY() == C.GetY())
            hallwayArea += (B.GetX() - A.GetX() + C.GetX() - D.GetX())*(D.GetY() - A.GetY())/2;
        else // a jeśli nie, to zwraca błąD.
            return -1;

        i++;
    }

    return hallwayArea;
}

/*
 *  Obliczanie pola powierzchni magazynu.
 */
double TrapezeWarehouse::GetWarehouseArea()
{
    // punkt przecięcia podstawy i lewego boku
    double Ax = 0;
    // double Ay = 0;

    // punkt przecięcia podstawy i prawego boku
    double Bx = - rightLegB / rightLegA;
    // double By = 0;

    // punkt przecięcia lewego i prawego boku
    double Ex = rightLegB / ( leftLegA - rightLegA ) ;
    double Ey = leftLegA * Ex;

    if(Ey <= warehouseHeight)
        return (Bx - Ax) * Ey / 2;

    // punkt przecięcia prawego boku i górnej podstawy
    double Cx = (warehouseHeight - rightLegB) /  rightLegA ;
    // double Cy = warehouseHeight;

    // punkt przecięcia lewego boku i górnej podstawy
    double Dx = warehouseHeight /  leftLegA ;
    // double Dy = warehouseHeight;

    return ((Bx - Ax) + (Cx - Dx))*warehouseHeight/2;
}
