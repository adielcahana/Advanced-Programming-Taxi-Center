#include <stdexcept>
#include <sstream>
#include "Map.h"
using namespace std;
/******************************************************************************
* The Function Operation: map ctor, create hash table for obstacles
******************************************************************************/
Map::Map(int width, int length, vector<Point*>* obstacles){
    this->width = width;
    this->length = length;
    this->obstacles = obstacles;
    this->obstaclesHash.resize((unsigned int) Point(width, length).hash());
    if (obstacles != NULL) {
        for (int i = 0; i < obstacles->size(); i++) {
            this->obstaclesHash[obstacles->at(i)->hash()] = 1;
        }
    }
}

/******************************************************************************
* The Function Operation: retrive the vertical and horizontal
* neihgbors of a given point
******************************************************************************/
queue<Node*>* Map::getAdjacent(const Node *point) {
    Point* temp;
    if (!isInMap((Point*) point)) {
        throw out_of_range("getAdjacent args are out of bounds!");
    }
	queue<Node*>* adjacents = new queue<Node*>();
	int x = ((Point*) point)->getX();
	int y = ((Point*) point)->getY();
	//verify which are Point's neighbors, check if there is obstacle in their place
    //and add them to adjacents
    if (x - 1 >= 0) {
        temp = new Point(x - 1, y);
        if (obstaclesHash.at((unsigned int) temp->hash()) != 1) {
            adjacents->push(temp);
        } else {
            delete temp;
        }
    }
    if (y + 1 < length) {
        temp = new Point(x , y + 1);
        if (obstaclesHash.at((unsigned int) temp->hash()) != 1){
            adjacents->push(temp);
        } else {
            delete temp;
        }
    }
    if (x + 1 < width) {
        temp = new Point(x + 1, y);
        if(obstaclesHash.at((unsigned int) temp->hash()) != 1){
            adjacents->push(temp);
        } else {
            delete temp;
        }
    }
    if (y - 1 >= 0) {
        temp = new Point(x , y - 1);
        if(obstaclesHash.at((unsigned int) temp->hash()) != 1) {
            adjacents->push(temp);
        } else {
        delete temp;
        }
    }
	return adjacents;
}
/******************************************************************************
* The Function Operation: retrive the shortest route
* between two points in the map
******************************************************************************/
vector<Point*>* Map::getRoute(Point *start, Point *end){
    if (!isInMap(start) || !isInMap(end)) {
        throw out_of_range("getRoute args are out of bounds!");
    }
	//create max point availble in the grid, which has the highest hash value
    Node* maxPoint = new Point(width, length);
	//send max point to bfs, in order to allocate a large enough hash table
	Bfs bfs(maxPoint);
    delete maxPoint;
    PointComparator comparator;
	return (vector<Point*>*) bfs.find_shortest_route(this,
                                                     start, end, &comparator);
}
/******************************************************************************
* The Function Operation: decide on whether point is in the map boundarys
******************************************************************************/
bool Map::isInMap(Point *p){
    int x = p->getX();
    int y = p->getY();
	//verify that x and y are between valid values of the map
    return (x >= 0 && x < width && y >= 0 && y < length);
}
/******************************************************************************
* The Function Operation: deserialize a Map object from text width_length
******************************************************************************/
Map* Map::deserialize(string s){
    vector<char*> parts;
    char *input = new char[s.length()];
    strcpy(input, s.c_str());
    char* part = strtok(input," ");
    parts.push_back(part);
    while((part = strtok(NULL," ")) != NULL){
        parts.push_back(part);
    }
    Point* maxPoint = Point::deserialize(parts.at(0));
    int size = atoi(parts.at(1));
    vector<Point*>* obstacles = new vector<Point*>();
    for(int i = 2; i < size; i++){
        obstacles->push_back(Point::deserialize(parts.at(i)));
    }
    Map* map = new Map(maxPoint->getX(),maxPoint->getY(), obstacles);
    delete maxPoint;
    delete input;
    return map;
}
/******************************************************************************
* The Function Operation: serialize a Map object to string
******************************************************************************/
string Map::serialize(){
    stringstream strs;
    strs << width << "," << length << " ";
    strs << obstacles->size();
    for (int i = 0; i < obstacles->size(); i++){
        strs << " " << obstacles->at(i)->serialize();
    }
    return strs.str();
}

Map::~Map() {
    for(int i = 0; i < obstacles->size(); i++){
        delete obstacles->at(i);
    }
    delete this->obstacles;
}

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
const char* Point::serialize() {
    stringstream strs;
    strs << x << "," << y;
    return strs.str().c_str();
}

/******************************************************************************
* The Function Operation: evaluate if 2 points are equal by their == operator
* used in order to provide a clean downcast and code writing
******************************************************************************/
bool PointComparator::equals(const Node *n1, const Node *n2) const {
    // downcast to Point, in order to use Point '==' operator
    return   *(Point*) n1 == *(Point*) n2;
}
