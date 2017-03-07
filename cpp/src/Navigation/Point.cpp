//
// Created by adi on 26/01/17.
//

#include "Point.h"

/******************************************************************************
* The Function Operation: copy ctor
******************************************************************************/
Point::Point(Point& p){
    this->x = p.getX();
    this->y = p.getY();
}
/******************************************************************************
* The Function Operation: return x axis value of the point
******************************************************************************/
int Point::getX(){
    return this->x;
}
/******************************************************************************
* The Function Operation: return y axis value of the point
******************************************************************************/
int Point::getY(){
    return this->y;
}
/******************************************************************************
* The Function Operation: return an integer value for a point with cantor's
* diagonal proof method
******************************************************************************/
int Point::hash(){
    int x = this->getX();
    int y = this->getY();
    //map (x,y) to an integer number
    return ((x + y) * (x + y + 1)) / 2 + x;
}

/******************************************************************************
* The Function Operation: evaluate if 2 points are equal by their x,y values
******************************************************************************/
bool Point::operator==(const Point &other) const {
    return x == other.x && y == other.y;
}

/******************************************************************************
* The Function Operation: evaluate if 2 points are not equal by their x,y values
******************************************************************************/
bool Point::operator!=(const Point &other) const {
    return !(other == *this);
}

/******************************************************************************
* The Function Operation: deserialize a Point object from text x,y
******************************************************************************/
Point* Point::deserialize(string s) {
    char *c = new char[s.length() + 1];
    strcpy(c, s.c_str());
    char *x, *y;
    //verify x and y are only digits
    try {
        if ((x = strtok(c, ",")) == NULL) throw runtime_error("bad arguments for point");
        for (int i = 0; x[i] != 0; i++) {
            if (!isdigit(x[i])) throw runtime_error("bad arguments for point");
        }
        if ((y = strtok(NULL, ",")) == NULL) throw runtime_error("bad arguments for point");
        for (int i = 0; y[i] != 0; i++) {
            if (!isdigit(y[i])) throw runtime_error("bad arguments for point");
        }
    }
    catch (exception){
        delete[](c);
        throw runtime_error("bad arguments for point");
    }
    Point* p = new Point(atoi(x), atoi(y));
    delete[] (c);
    return p;
}

/******************************************************************************
* The Function Operation: serialize a Point object to text x,y
******************************************************************************/
string Point::toString() {
    stringstream strs;
    strs << x << "," << y;
    return strs.str();
}

/******************************************************************************
* The Function Operation: evaluate if 2 points are equal by their == operator
* used in order to provide a clean downcast and code writing
******************************************************************************/
bool PointComparator::equals(const Node *n1, const Node *n2) const {
    // downcast to Point, in order to use Point '==' operator
    return   *(Point*) n1 == *(Point*) n2;
}

