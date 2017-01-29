#include "Trip.h"

/******************************************************************************
* The function Operation: Trip constructor.
******************************************************************************/
Trip::Trip(int id, Point start, Point end, int numOfPass, double price, int time):
        id(id), start(start), end(end), numOfPass(numOfPass), price(price),
        time(time), route(NULL), totalMeterPassed(0) {
    for (int i = 0; i < this->numOfPass; i++) {
        passengers.push_back(new Passenger());
    }
}

/******************************************************************************
* The function Operation: Trip destructor - delete the route if it's exist
* delete the passengers
******************************************************************************/
Trip::~Trip() {
    if(this->route != NULL){
        for (int i = 0; i < route->size(); i++){
            delete route->at(i);
        }
        route->clear();
        delete route;
    }

    for(int j = 0; j < passengers.size(); j++){
        delete passengers.at(j);
    }
    passengers.clear();
}

/******************************************************************************
* The function Operation: return the trip id
******************************************************************************/
int Trip::getId() {
    return this->id;
}

/******************************************************************************
* The function Operation: add meter to the total meter that passed
******************************************************************************/
void Trip::addMeter(){
    this->totalMeterPassed += 1;
}

/******************************************************************************
* The function Operation: get the next point from the route copy it delete the
* point from the route and return the copy
******************************************************************************/
Point* Trip::getNextPoint(){
    if (route->size() != 0) {
        Point* nextPoint = this->route->front();
        //safe copy
        nextPoint = new Point(*nextPoint);
        // add meter one passed
        this->addMeter();
        delete route->front();
        this->route->erase(route->begin());
        return nextPoint;
    }
    // if route is empty
    return NULL;
}

/******************************************************************************
* The function Operation: return the trip's price
******************************************************************************/
double Trip::getPrice() {
    return this->price;
}

/******************************************************************************
* The function Operation: return the trip's number of passengers
******************************************************************************/
int Trip::getNumPassengers(){
    return numOfPass;
}

/******************************************************************************
* The function Operation: calculate the sum of passengers's satisfacation and
* return it
******************************************************************************/
int Trip::sumOfSatisfaction() {
    int sum = 0;
    for (int i = 0; i < this->passengers.size(); i++){
        sum += this->passengers.at(i)->satisfacation();
    }
    return sum;
}

/******************************************************************************
* The function Operation: serialize a trip
******************************************************************************/
string Trip::toString() {
    stringstream strs;
    // serialize trip id
    strs << this->id;
    strs << " ";
    // serialize start point
    strs << this->start.toString();
    strs << " ";
    // serialize end point
    strs << this->end.toString();
    strs << " ";
    //serialize the route
    strs << route->size();
    for (int i = 0; i < route->size(); i++) {
        strs << " " << route->at(i)->toString();
    }
    strs << " ";
    // serialize trip num of passengers
    strs << this->numOfPass;
    strs << " ";
    // serialize trip price
    strs << this->price;
    strs << " ";
    // serialize trip time
    strs << this->time;
    return strs.str();
}
/******************************************************************************
* The function Operation: deserialize a trip
******************************************************************************/
Trip* Trip::fromString(string s) {
    stringstream strs(s);
    stringbuf buff;
    int id = 0, numOfPass = 0, route_size = 0, time = 0;
    char dummy;
    double price = 0;
    vector <Point*>* route = NULL;
    strs >> id;
    strs >> noskipws >> dummy;
    // deserialize start point
    strs.get(buff, ' ');
    Point* start = Point::deserialize(buff.str());
    buff.str("");
    // serialize end point
    strs >> noskipws >> dummy;
    strs.get(buff, ' ');
    Point* end = Point::deserialize(buff.str());
    buff.str("");
    //deserialize the route
    strs >> noskipws >> dummy;
    strs >> route_size;
    if (route_size > 0) {
        route = new vector <Point*>();
        for (int i = 0; i < route_size; i++) {
            strs >> noskipws >> dummy;
            strs.get(buff, ' ');
            Point* temp = Point::deserialize(buff.str());
            route->push_back(temp);
            buff.str("");
        }
    }
    // deserialize trip num of passengers
    strs >> noskipws >> dummy;
    strs >> numOfPass;
    // deserialize trip price
    strs >> noskipws >> dummy;
    strs >> price;
    // serialize trip time
    strs >> noskipws >> dummy;
    strs >> time;
    Trip* trip = new Trip(id, *start, *end, numOfPass, price, time);
    delete start;
    delete end;
    trip->route = route;
    return trip;
}
/******************************************************************************
* The function Operation: returns the trip start time
******************************************************************************/
int Trip::getTime() {
    return this->time;
}
/******************************************************************************
* The function Operation: returns a copy of the trip start point
******************************************************************************/
Point* Trip::getStart(){
    return new Point(start);
}
/******************************************************************************
* The function Operation: returns a copy of the trip destanation point
******************************************************************************/
Point* Trip::getEnd(){
    return new Point(end);
}
/******************************************************************************
* The function Operation: returns the route size.
* if there is no route, returns -1 (error)
******************************************************************************/
int Trip::size() {
    if (route != NULL){
        return route->size();
    }
    //error
    return -1;
}
/******************************************************************************
* The function Operation: set the trip route. the function creates a safe
* copy of the given route vector
******************************************************************************/
void Trip::setRoute(vector<Point *> *route) {
    this->route = new vector<Point *>(*route);
}