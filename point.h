#ifndef POINT_H
#define POINT_H


class Point
{
private:
    double x; // pierwsza współrzędna
    double y; // druga współrzędna
public:
    Point(double x, double y) : x(x), y(y) {}
    void AddToX(double a) { x += a; }
    void AddToY(double a) { y += a; }
    double GetX(){return x;}
    double GetY(){return y;}
    void SetX( double new_x ){x = new_x;}
    void SetY( double new_y ){y = new_y;}
    friend bool operator==(Point& p1, Point& p2) { return p1.x == p2.x && p1.y == p2.y;}
};

#endif // POINT_H
