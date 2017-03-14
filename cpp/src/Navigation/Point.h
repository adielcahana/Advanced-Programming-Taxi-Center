//
// Created by adi on 26/01/17.
//

#ifndef EX4_POINT_H
#define EX4_POINT_H

#include "Bfs.h"
#include <cstring>
#include <sstream>
#include <stdexcept>

using namespace std;
/******************************************************************************
* Point: defines a 2d vector, that represnt a location on the map. implements
* Node interface in order to be usable by the Bfs algorithm
******************************************************************************/
class Point: public Node{
private:
    int x;
    int y;
public:
    Point(int x, int y): x(x), y(y) {};
    Point(Point &p);
    int getX();
    int getY();
    int hash();
    bool operator==(const Point &rhs) const;
    bool operator!=(const Point &rhs) const;
    static Point* deserialize(string s);
    string toString();
    /**********************************************************************
    * The Function Operation: print a represantation of a 2d point
    * we were unable to locate the function in the .cpp file
    ***********************************************************************/
    friend ostream& operator<<( ostream& output,const Point& p) {
        output << "(" << p.x << "," << p.y << ")";
        return output;
    }
};

/******************************************************************************
* PointComparator: defines a comparing method for points.
* this class provides a clean way to downcast Node pointer to Point.
* implements NodeComparator in order to provide a comparing method for
* the Bfs algorithm
******************************************************************************/
class PointComparator: public NodeComparator{
public:
    virtual bool equals(const Node *n1, const Node *n2) const;
};


#endif //EX4_POINT_H
