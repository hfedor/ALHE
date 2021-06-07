//
// Created by Michal on 18.05.2021.
//

#ifndef UNTITLED3_WARE_H
#define UNTITLED3_WARE_H

/*
 *  Klasa obiektów przedstawiających towary
*/
class Ware {
public:
    int id;                 // indeks towaru
    double width;           // szerokość towaru
    double height;          // wysokość  towaru
    double x;               // pierwsza współrzędna położenia towaru w magazynie - (lewy górny wierzchołek towaru)
    double y;               // druga    współrzędna położenia towaru w magazynie - (lewy górny wierzchołek towaru)
    bool rotate = false;    // czy towar został obrócony o 90 stopi
    bool fitted = false;    // czy towar został umieszczony w magazynie

    Ware(int id,double width,double height)
    {
        this->id = id;
        this->height = height;
        this->width = width;
        this->fitted=false;
        this->rotate=false;
        this->x=-1;
        this->y=-1;
    }
    Ware()
    {}
    double getActualWidth()
        { return rotate ? height : width; }
    double getActualHeight()
        { return rotate ?   width : height; }
    double getArea()
        { return width * height; }
};


#endif //UNTITLED3_WARE_H
