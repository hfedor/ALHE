//
// Created by Michal on 18.05.2021.
//

#ifndef UNTITLED3_WARE_H
#define UNTITLED3_WARE_H


class Ware {
public:
    int id;
    double width;
    double height;
    double x;
    double y;
    bool rotate = false;
    bool fitted = false;

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
