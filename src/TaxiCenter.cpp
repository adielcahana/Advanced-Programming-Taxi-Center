#include <unistd.h>
#include "TaxiCenter.h"

/******************************************************************************
* The function Operation: TaxiCenter constructor.
******************************************************************************/
TaxiCenter::TaxiCenter(Protocol *protocol, Tcp *tcp, Map *map) : Server(protocol, tcp) {
    avaliableDrivers = new vector <Comunicator*>;
    avaliableCabs = new vector <Taxi*>();
    trips = new vector <Trip*>();
    uncalculatedtrips = new queue <Trip*>();
    this->map = map;
    if (pthread_mutex_init(&lock, NULL) != 0) {
        cout << "unable to init taxi center lock" << endl;
    }
}

/******************************************************************************
* The function Operation: TaxiCenter destructor - delete all the drivers
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
        sleep(1);
    }
    for(int i = 0; i < avaliableDrivers->size(); i++){
        delete avaliableDrivers->at(i);
    }
    delete trips;
    delete avaliableDrivers;
    avaliableCabs->clear();
    delete avaliableCabs;
    delete (TaxiCenterProtocol *) this->protocol;
    delete map;
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
    this->trips->push_back(trip);
    this->uncalculatedtrips->push(trip);
    trip->setThread(new pthread_t());
    int status = pthread_create(trip->getThread(), NULL, TaxiCenter::wrapCreateRoute, this);
    if (status) {
        cout << "error while trying to create trip" << endl;
    }
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
        sleep(1);
    }
    can_continue = false;
    for(int i = 0; i < avaliableDrivers->size(); i++){
        avaliableDrivers->at(i)->setNextMission(5);
    }
    can_continue = true;
}

/******************************************************************************
* The function Operation: ask for driver location and retrun it
******************************************************************************/
Point* TaxiCenter::getDriverLocation(int driverId) {
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(1);
    }
    can_continue = false;
    for(int i = 0; i < avaliableDrivers->size(); i++){
        Comunicator* driver = avaliableDrivers->at(i);
        if(driver->getDriverId() == driverId) {
            avaliableDrivers->at(i)->setNextMission(6);
            can_continue = true;
            while(avaliableDrivers->size() != numOfDrivers){
                sleep(1);
            }
            return driver->getLocation();
        }
    }
}

/******************************************************************************
* The function Operation: send trip to the driver
******************************************************************************/
void TaxiCenter::addTripToDriver(int time){
    Trip* trip = NULL;
    for(unsigned long i = 0; i < this->trips->size(); i++) {
        // if the time of the trip arrived
        trip = this->trips->at(i);
        if (time >= trip->time) {
            if (trip->route == NULL) {
                pthread_t *thread = trip->getThread();
                pthread_join(*thread, NULL);
                delete thread;
                trip->setThread(NULL);
            }
            this->trips->erase(trips->begin() + i);
            break;
        }
    }
    if (trip != NULL){
        Comunicator* driver = this->getClosestDriver(trip->start);
        driver->setTrip(trip);
        // send trip to driver
        driver->setNextMission(4);
        // get message that trip accepted
        // delete the trip from the taxi center list
        delete trip;
    }
}

void TaxiCenter::addComunicator(Comunicator *comunicator) {
    this->avaliableDrivers->push_back(comunicator);
}

void TaxiCenter::acceptNewDriver() {
    Tcp* tcp = this->accept();
    Comunicator* comunicator = new Comunicator(new TaxiCenterProtocol(), tcp, this->map);
    this->addComunicator(comunicator);
    //run communicatio in seperate thread
    comunicator->setThread(new pthread_t());
    int status = pthread_create(comunicator->getThread(), NULL, Comunicator::wrapTalkWithDriver, comunicator);
    if (status) {
        cout << "error while trying to comunicate" << endl;
    }

    while(avaliableDrivers->size() != numOfDrivers){
        sleep(1);
    }
    Taxi* taxi = getTaxiById(comunicator->getTaxiId());
    comunicator->setTaxi(taxi);
}

void TaxiCenter::sendFinish() {
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(1);
    }
    can_continue = false;
    for(int i = 0; i < avaliableDrivers->size(); i++){
        avaliableDrivers->at(i)->setNextMission(7);
    }
    can_continue = true;
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(1);
    }
}

Comunicator* TaxiCenter::getClosestDriver(Point location){
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(1);
    }
    Comunicator* driver = NULL;
    for(int i = 0; avaliableDrivers->size(); i++ ){
        driver = avaliableDrivers->at(i);
        if (driver->isAvaliable()){
            if (location == *getDriverLocation(driver->getDriverId())){
                return driver;
            }
        }
    }
}

void* TaxiCenter::wrapCreateRoute(void* center){
    ((TaxiCenter*) center)->createRoute();
    pthread_exit(NULL);
}