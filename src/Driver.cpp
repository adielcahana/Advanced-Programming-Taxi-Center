#include <sstream>
#include "Driver.h"
#include "Parser.h"

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
* The function Operation: Driver destructor - delete the trip and the location.
******************************************************************************/
Driver::~Driver(){
    if (trip != NULL) {
        delete trip;
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
    notifyUnavaliable();
    Point* start = new Point(trip->start);
    Point* end = new Point(trip->end);
    trip->route = map->getRoute(start, end);
    this->trip = trip;
}

/******************************************************************************
* The function Operation: if the driver have trip, move to next point.
******************************************************************************/
void Driver::timePassed(){
    if(trip == NULL){
        return;
    }
    moveOneStep();
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
            // the driver is available
            notifyAvaliable();
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
* The function Operation: add the driver to the available vector
******************************************************************************/
void Driver::addAvaliableListener(AvaliableListener* al){
    this->avaliableListeners.push_back(al);
}

/******************************************************************************
* The function Operation: remove the driver from the available vector
******************************************************************************/
void Driver::removeAvaliableListener(AvaliableListener* al){
    for(int i = 0; i< avaliableListeners.size(); i++){
        if(avaliableListeners.at(i) == al){
            avaliableListeners[i] = NULL;
            avaliableListeners.erase(avaliableListeners.begin() + i);
        }
    }
}

/******************************************************************************
* The function Operation: notify that the driver is available
******************************************************************************/
void Driver::notifyAvaliable(){
    for(int i = 0; i< avaliableListeners.size(); i++){
        avaliableListeners[i]->avaliableEvent(this);
    }
}

/******************************************************************************
* The function Operation: notify that the driver is not available
******************************************************************************/
void Driver::notifyUnavaliable(){
    for(int i = 0; i< avaliableListeners.size(); i++){
        avaliableListeners[i]->unavaliableEvent(this);
    }
}

bool Driver::isAvaliable(){
    return (trip == NULL);
}
void Driver::setTaxiCenter(const char* ip_address, const int port_no){
    setConnnection(ip_address, port_no);
}

int main(int argc, char** argv) {
    Parser pars;
    Driver *driver = pars.readDriver();
    driver->setTaxiCenter(argv[1], atoi(argv[2])); //set the client connection
    int operation = 1;
    driver->send(operation); //send driver id
    do {
        operation = driver->receive(1); //confirm msg delivery
        driver->send(operation); //send confirmation or ask again
    } while (operation == 0);
    Map* map = NULL;
    while(map == NULL){
        driver->receive(0); //recieve taxi
        try {
            map = Map::deserialize(driver->buffer);
            driver->setMap(map);
        } catch (runtime_error){
            driver->send(0); // request data again
        }
    }
    driver->send(3);
    Taxi* taxi = NULL;
    while(taxi == NULL){
        driver->receive(0); //recieve taxi
        istringstream stream(driver->buffer);
        streambuf *cin_backup = cin.rdbuf(stream.rdbuf());
        cin.rdbuf(stream.rdbuf()); //redirect std::cin
        try {
            taxi = pars.readTaxi();
            driver->setTaxi(taxi);
        } catch (runtime_error){
            driver->send(0); // request data again
        }
        cin.rdbuf(cin_backup);
    }
    while(true) {
        Trip *trip = NULL;
        driver->send(4);
        while (trip == NULL) {
            driver->receive(0); //recieve trip
            istringstream stream(driver->buffer);
            streambuf *cin_backup = cin.rdbuf(stream.rdbuf());
            cin.rdbuf(stream.rdbuf()); //redirect std::cin
            try {
                trip = pars.readTrip();
                driver->newTrip(trip);
            } catch (runtime_error) {
                driver->send(0); // request data again
            }
            cin.rdbuf(cin_backup);
        }
        driver->send(5);
        while (!driver->isAvaliable()) {
            operation = driver->receive(4);
            if (operation == 5) {
                driver->timePassed();
            }
            driver->send(operation);
            if (driver->isAvaliable()) {
                driver->send(7);
            }
        }
    }
}