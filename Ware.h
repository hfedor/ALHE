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

    double getActualWidth()
        { return rotate ? height : width; }
    double getActualHeight()
        { return rotate ?   width : height; }
    double getArea()
        { return width * height; }
};


#endif //UNTITLED3_WARE_H
