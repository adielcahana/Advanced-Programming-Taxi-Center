#include "Driver.h"

/******************************************************************************
* The function Operation: calculate the average stisfaction of the driver.
******************************************************************************/
void Driver::calcAvg(){
    // total stisfaction
    float SumOfsatisfaction = (avgSatisfaction * passCount) + trip->sumOfSatisfaction();
    // add stisfaction from the last passengers
    addPassCount(trip->getNumPassengers());
    avgSatisfaction = SumOfsatisfaction / passCount;
}

/******************************************************************************
* The function Operation: Driver destructor - delete the trip and the location,
* delete the map and the taxi if they exist
******************************************************************************/
Driver::~Driver(){
    if (trip != NULL) {
        delete trip;
    }
    if (map != NULL) {
        delete map;
    }
    if (taxi != NULL) {
        delete taxi;
    }
    if (location != NULL) {
        delete location;
    }
}

/******************************************************************************
* The function Operation: add the last passengers to the total passenger count.
******************************************************************************/
void Driver::addPassCount(int passengers){
    passCount += this->trip->getNumPassengers();
}

/******************************************************************************
* The function Operation: add to the driver new trip and notify that the
* driver is not available.
******************************************************************************/
void Driver::newTrip(Trip* trip){
    this->trip = trip;
}

/******************************************************************************
* The function Operation: if the driver have trip, move to next point.
******************************************************************************/
void Driver::timePassed(){
    time++;
    if(trip == NULL){
        return;
    }
    if (trip->getTime() < time){
        moveOneStep();
    }
}

/******************************************************************************
* The function Operation: the driver move to next point if he have lux taxi he
* move 2 steps.
******************************************************************************/
void Driver::moveOneStep() {
    Point *nextPoint = NULL;
    for (int i = 0; i < this->getTaxi()->getVelocity(); i++) {
        nextPoint = this->trip->getNextPoint();
        if (nextPoint == NULL) {
            // if the trip over calculate stisfaction and delete trip
            this->calcAvg();
            delete trip;
            this->trip = NULL;
            delete nextPoint;
            return;
        }
        delete this->location;
        // move to next point and update the taxi location
        this->location = nextPoint;
        this->getTaxi()->moveOneStep(nextPoint);
    }
}

/******************************************************************************
* The function Operation: return the driver location
******************************************************************************/
Point * Driver::getLocation(){
    return new Point(*this->location);
}

/******************************************************************************
* The function Operation: return the driver taxi id
******************************************************************************/
int Driver::getTaxiId(){
    return this->taxiId;
}

/******************************************************************************
* The function Operation: return the driver taxi
******************************************************************************/
Taxi* Driver::getTaxi() {
    return this->taxi;
}

/******************************************************************************
* The function Operation: set the world map
******************************************************************************/
void Driver::setMap(Map* map) {
    this->map = map;
}

/******************************************************************************
* The function Operation: update the driver's taxi
******************************************************************************/
void Driver::setTaxi(Taxi* taxi){
    this->taxi = taxi;
}

/******************************************************************************
* The function Operation: return the driver id
******************************************************************************/
int Driver::getId() {
    return this->id;
}

/******************************************************************************
* The function Operation: return if the driver avaliable
******************************************************************************/
bool Driver::isAvaliable(){
    return (trip == NULL);
}
