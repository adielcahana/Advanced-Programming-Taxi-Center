#include <unistd.h>
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

    this->trips->push_back(trip);
    this->uncalculatedtrips->push(trip);
//    trip->setThread(new pthread_t());
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
//            stringstream str;
//            str << *location;
//            LDEBUG << str.str();
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
    Trip* trip = NULL;
    for(unsigned long i = 0; i < this->trips->size(); i++) {
        // if the time of the trip arrived
        trip = this->trips->at(i);
        if (time >= trip->time) {
            if (trip->route == NULL) {
                pthread_t* thread = trip->getThread();
                pthread_join(*thread, NULL);
            }
            Comunicator* driver = this->getClosestDriver(trip->start);
            driver->setTrip(trip);
            this->trips->erase(trips->begin() + i);
            i--;
            // send trip to driver
            driver->setNextMission(4);
            // get message that trip accepted
        }
    }
    //wait for all drivers to get their trips
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(SLEEP);
    }
}

void TaxiCenter::addComunicator(Comunicator *comunicator) {
    this->drivers->push_back(comunicator);
}

void TaxiCenter::acceptNewDriver() {
    Tcp* tcp = this->accept();
    this->numOfDrivers++;
    Comunicator* comunicator = new Comunicator(new TaxiCenterProtocol(), tcp, this->map);
    //run communicatio in seperate thread
    this->addComunicator(comunicator);
    comunicator->addAvaliableListener(this->listener);
    comunicator->setThread(new pthread_t());
    int status = pthread_create(comunicator->getThread(), NULL, Comunicator::wrapTalkWithDriver, comunicator);
    if (status) {
        cout << "error while trying to comunicate" << endl;
    }
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(SLEEP);
    }
    can_continue = false;
    Taxi* taxi = getTaxiById(comunicator->getTaxiId());
    comunicator->setTaxi(taxi);
    comunicator->setNextMission(3);
    can_continue = true;
}

void TaxiCenter::sendFinish() {
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(SLEEP);
    }
    can_continue = false;
    for(int i = 0; i < numOfDrivers; i++){
        drivers->at(i)->setNextMission(7);
    }
    can_continue = true;
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(SLEEP);
    }
}

Comunicator* TaxiCenter::getClosestDriver(Point location){
    Comunicator* driver = NULL;
    for(unsigned int i = 0; i < drivers->size(); i++ ){
        driver = drivers->at(i);
        if (driver->isAvaliable()){
            driver->setNextMission(6);
            Point* driverLocation = driver->getLocation();
            if (location == *driverLocation){
                delete driverLocation;
                return driver;
            }
            delete driverLocation;
        }
    }
}

void* TaxiCenter::wrapCreateRoute(void* center){
    ((TaxiCenter*) center)->createRoute();
    pthread_exit(NULL);
}
