#ifndef POINT_H
#define POINT_H


class Point
{
private:
    double x;
    double y;
public:
    // CONSTRUCTORS
    Point(double x, double y);
    // GETTERS
    double GetX(){return x;}
    double GetY(){return y;}
};

#endif // POINT_H
