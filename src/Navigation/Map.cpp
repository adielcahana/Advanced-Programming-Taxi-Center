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
    char *input = new char[s.length() + 1];
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
    delete[](input);
    return map;
}

/******************************************************************************
* The Function Operation: serialize a Map object to string
******************************************************************************/
string Map::toString(){
    stringstream strs;
    strs << width << "," << length << " ";
    if (obstacles != NULL) {
        strs << obstacles->size();
        for (int i = 0; i < obstacles->size(); i++) {
            strs << " " << obstacles->at(i)->toString();
        }
    } else {
        strs << "0";
    }
    return strs.str();
}

/******************************************************************************
* The Function Operation: Map destructor
******************************************************************************/
Map::~Map() {
    for(int i = 0; i < obstacles->size(); i++){
        delete obstacles->at(i);
    }
    delete this->obstacles;
}

/******************************************************************************
* The Function Operation: check if given trip is in the map range
******************************************************************************/
bool Map::isTripInMap(Trip* trip){
    Point* start = trip->getStart();
    Point* end = trip->getEnd();
    if (!isInMap(start) || !isInMap(end)) {
        delete start;
        delete end;
        return false;
    }
    delete start;
    delete end;
    return true;
}
