#include "TaxiCenter.h"

/******************************************************************************
* The function Operation: TaxiCenter constructor.
******************************************************************************/
TaxiCenter::TaxiCenter(Map* map){
    drivers = new vector <Driver*>;
    avaliableDrivers = new vector <Driver*>;
    avaliableCabs = new vector <Taxi*>();
    avaliableDriversListener = new TripListener(avaliableDrivers);
    this->map = map;
}

/******************************************************************************
* The function Operation: TaxiCenter destructor - delete all the drivers
* delete the avaliable cabs, delete the listeners and the map
******************************************************************************/
TaxiCenter::~TaxiCenter() {
    for(int i = 0; i < drivers->size(); i++){
        delete drivers->at(i)->getTaxi();
        delete drivers->at(i);
    }
    for(int i = 0; i < avaliableCabs->size(); i++){
        delete avaliableCabs->at(i);
    }
    delete drivers;
    delete avaliableDrivers;
    avaliableCabs->clear();
    delete avaliableCabs;
    delete avaliableDriversListener;
    delete map;
}

/******************************************************************************
* The function Operation: get a driver and add him to the driver's vector
* give the driver a cab by his taxi id
******************************************************************************/
void TaxiCenter::addDriver(Driver *driver){
    this->drivers->push_back(driver);
    Taxi* taxi = searchTaxiById(driver->getTaxiId());
    driver->setTaxi(taxi);
    // the driver is avaliable
    driver->addAvaliableListener(avaliableDriversListener);
    avaliableDrivers->push_back(driver);
}

/******************************************************************************
* The function Operation: get a taxi and add him to the avaliable cabs
******************************************************************************/
void TaxiCenter::addAvaliableTaxi(Taxi *taxi){
    this->avaliableCabs->push_back(taxi);
}

/******************************************************************************
* The function Operation: get an id and return the taxi with the same id if
* it in the avaliable cabs vector, else return NULL
******************************************************************************/
Taxi* TaxiCenter::searchTaxiById(int id){
    Taxi* taxi = NULL;
    for(int i = 0; i < this->avaliableCabs->size(); i++){
        taxi = this->avaliableCabs->at(i);
        if(taxi->getId() == id){
            taxi = this->avaliableCabs->at(i);
            this->avaliableCabs->erase(this->avaliableCabs->begin() + i);
            return taxi;
        }
    }
    return NULL;
}

/******************************************************************************
* The function Operation: if there is a driver in the trip start point the
* driver get the trip
******************************************************************************/
void TaxiCenter::notifyNewTrip(Trip* trip){
    Driver *driver = NULL;
    Point* location = NULL;
    for(int i = 0; i < this->avaliableDrivers->size(); i++){
        driver = this->avaliableDrivers->at(i);
        location = driver->getLocation();
        if(trip->start == *location){
            driver->newTrip(trip);
            delete location;
            return;
        }
        delete location;
    }
    delete trip;
}

/******************************************************************************
* The function Operation: get a trip create a route from it's start and end
* points and check if some driver can get this trip
******************************************************************************/
void TaxiCenter::addTrip(Trip* trip){
    Point* start = new Point(trip->start);
    Point* end = new Point(trip->end);
    trip->route = map->getRoute(start, end);
    notifyNewTrip(trip);
    delete end;
}

/******************************************************************************
* The function Operation: notify all the driver that time passed
******************************************************************************/
void TaxiCenter::timePassed(){
    for(int i = 0; i < drivers->size(); i++){
        drivers->at(i)->timePassed();
    }
}

/******************************************************************************
* The function Operation: check if all the driver is avaliable
******************************************************************************/
bool TaxiCenter::shouldStop(){
    return (avaliableDrivers->size() == drivers->size());
}

/******************************************************************************
* The function Operation: get driver location by id
******************************************************************************/
Point * TaxiCenter::getDriverLocation(int id){
    Point* point = NULL;
    Driver *driver = NULL;
    for(int i = 0; i < this->drivers->size(); i++){
        driver = this->drivers->at(i);
        if(driver->getId() == id){
            point = driver->getLocation();
            return point;
        }
    }
    return NULL;
}