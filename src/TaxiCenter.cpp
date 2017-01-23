#include <unistd.h>
#include <climits>
#include "TaxiCenter.h"

/******************************************************************************
* The function Operation: TaxiCenter constructor.
******************************************************************************/
TaxiCenter::TaxiCenter(Protocol *protocol, Tcp *tcp, Map *map) : Server(protocol, tcp) {
    avaliableDrivers = new vector <Comunicator*>;
    drivers = new vector <Comunicator*>;
    avaliableCabs = new vector <Taxi*>();
    trips = new vector <Trip*>();
    uncalculatedtrips = new queue <Trip*>();
    this->map = map;
    if (pthread_mutex_init(&lock, NULL) != 0) {
        cout << "unable to init taxi center lock" << endl;
    }
    listener = new CommunicatorListener(this->avaliableDrivers );
    this->numOfDrivers = 0;
}

/******************************************************************************
* The function Operation: TaxiCenter destructor - delete all the comunicators
* delete the avaliable cabs, delete the listeners and the map
******************************************************************************/
TaxiCenter::~TaxiCenter() {
    for(int i = 0; i < avaliableCabs->size(); i++){
        delete avaliableCabs->at(i);
    }
    for(int i = 0; i < this->trips->size(); i++){
        delete this->trips->at(i);
    }
    while (avaliableDrivers->size() != numOfDrivers){
        sleep(SLEEP);
    }
    for(int i = 0; i < avaliableDrivers->size(); i++){
        delete avaliableDrivers->at(i);
    }
    delete trips;
    delete avaliableDrivers;
    delete drivers;
    avaliableCabs->clear();
    delete avaliableCabs;
    delete uncalculatedtrips;
    delete (TaxiCenterProtocol *) this->protocol;
    delete map;
    delete listener;
    pthread_mutex_destroy(&lock);
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
Taxi* TaxiCenter::getTaxiById(int id){
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
* The function Operation: add a trip to the trip list
******************************************************************************/
void TaxiCenter::addTrip(Trip* trip){
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    this->trips->push_back(trip);
    this->uncalculatedtrips->push(trip);
    trip->setThread(new pthread_t);
    int status = pthread_create(trip->getThread(), &attr, TaxiCenter::wrapCreateRoute, this);
    if (status) {
        cout << "error while trying to create trip" << endl;
    }
    pthread_attr_destroy(&attr);
}

/******************************************************************************
* The function Operation: get a trip  and create a route from it's start and end
* points
******************************************************************************/
void TaxiCenter::createRoute(){
    pthread_mutex_lock(&lock);
    Trip* trip = this->uncalculatedtrips->front();
    this->uncalculatedtrips->pop();
    cout << "trip num: " << trip->id << " is calcaulated" << endl;
    pthread_mutex_unlock(&lock);
    Point* start = new Point(trip->start);
    Point* end = new Point(trip->end);
    trip->route = map->getRoute(start, end);
    delete end;
    cout << "trip num: " << trip->id << " ended calcaulating" << endl;
}

/******************************************************************************
* The function Operation: notify all the driver that time passed
******************************************************************************/
void TaxiCenter::timePassed(){
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(SLEEP);
    }
    can_continue = false;
    for(int i = 0; i < numOfDrivers; i++){
        drivers->at(i)->setNextMission(5);
    }
    can_continue = true;
    //wait for all drivers to finish
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(SLEEP);
    }
}

/******************************************************************************
* The function Operation: ask for driver location and retrun it
******************************************************************************/
Point* TaxiCenter::getDriverLocation(int driverId) {
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(SLEEP);
    }
    can_continue = false;
    for(int i = 0; i < numOfDrivers; i++){
        Comunicator* driver = drivers->at(i);
        if(driver->getDriverId() == driverId) {
            driver->setNextMission(6);
            can_continue = true;
            Point* location = driver->getLocation();
            return location;
        }
    }
}

/******************************************************************************
* The function Operation: send trip to the driver
******************************************************************************/
void TaxiCenter::addTripToDriver(int time){
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(SLEEP);
    }
    vector <unsigned long> tripToDelete;
    Trip* trip = NULL;
    for(unsigned long i = 0; i < this->trips->size(); i++) {
        // if the time of the trip arrived
        trip = this->trips->at(i);
        if (time >= trip->time) {
            pthread_t* thread = trip->getThread();
            pthread_join(*thread, NULL);
            while (trip->route == NULL){
                sleep(SLEEP);
            }
            Comunicator* driver = this->getClosestDriver(trip->start);
            driver->setTrip(trip);
            tripToDelete.push_back(i);
            // send trip to driver
            driver->setNextMission(4);
            // get message that trip accepted
        }
    }
    for(int i = tripToDelete.size() - 1; i >= 0; i--){
        trips->erase(trips->begin() + tripToDelete.at(i));
    }
    //wait for all drivers to get their trips
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(SLEEP);
    }
}

/******************************************************************************
* The function Operation: add given comunicator to driver's list
******************************************************************************/
void TaxiCenter::addComunicator(Comunicator *comunicator) {
    this->drivers->push_back(comunicator);
}

/******************************************************************************
* The function Operation: get new Tcp from the taxi canter server create new
* comunicator and start to comunicate this driver in a different thread
******************************************************************************/
void TaxiCenter::acceptNewDriver() {
    Tcp* tcp = this->accept();
    this->numOfDrivers++;
    Comunicator* comunicator = new Comunicator(new TaxiCenterProtocol(), tcp, this->map);
    //run communicatio in seperate thread
    this->addComunicator(comunicator);
    comunicator->addAvaliableListener(this->listener);
    comunicator->setThread(new pthread_t);
    // start comunicate with the driver in different thread
    int status = pthread_create(comunicator->getThread(), NULL, Comunicator::wrapTalkWithDriver, comunicator);
    if (status) {
        cout << "error while trying to comunicate" << endl;
    }
    // sleep until all drivers avaliable
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(SLEEP);
    }
    can_continue = false;
    Taxi* taxi = getTaxiById(comunicator->getTaxiId());
    comunicator->setTaxi(taxi);
    comunicator->setNextMission(3);
    can_continue = true;
}

/******************************************************************************
* The function Operation: tell to all threads to send finish message to the
* drivers
******************************************************************************/
void TaxiCenter::sendFinish() {
    // sleep until all drivers avaliable
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(SLEEP);
    }
    can_continue = false;
    for(int i = 0; i < numOfDrivers; i++){
        drivers->at(i)->setNextMission(7);
    }
    can_continue = true;
    // sleep until all drivers avaliable
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(SLEEP);
    }
}

/******************************************************************************
* The function Operation: find the closest driver to send him the earlier trip
******************************************************************************/
Comunicator* TaxiCenter::getClosestDriver(Point location){
    Comunicator* driver = NULL;
    Comunicator* firstDriver = NULL;
    int min_time = INT_MAX;
    int driver_time = 0;
    for(unsigned int i = 0; i < drivers->size(); i++ ){
        driver = drivers->at(i);
        if (driver->isAvaliable()){
            // ask for driver's location
            driver->setNextMission(6);
            Point* driverLocation = driver->getLocation();
            // if the location is the same as trip start point
            if (location == *driverLocation){
                //find the first driver who got to the point
                driver_time = driver->getTime();
                if (driver_time < min_time){
                    min_time = driver_time;
                    firstDriver = driver;
                }
            }
            delete driverLocation;
        }
    }
    return firstDriver;
}

/******************************************************************************
* The function Operation: static function that called to createRoute function
* that the thread could run this function
******************************************************************************/
void* TaxiCenter::wrapCreateRoute(void* center){
    ((TaxiCenter*) center)->createRoute();
    // finish the thread
    pthread_exit(NULL);
}

Map *TaxiCenter::getMap() {
    return this->map;
}
