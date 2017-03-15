#include "TaxiCenter.h"

/******************************************************************************
* The function Operation: TaxiCenter constructor.
******************************************************************************/
TaxiCenter::TaxiCenter(Protocol *protocol, Tcp *tcp, Map *map) : Server(protocol, tcp) {
    tp = new ThreadPool(5);
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
    delete tp;
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
    Point* start = trip->getStart();
    Point* end = trip->getEnd();
    if(!this->map->isTripInMap(trip)){
        delete trip;
        delete start;
        delete end;
        throw out_of_range("getRoute args are out of bounds!");
    }
    delete start;
    delete end;
    this->trips->push_back(trip);
    this->uncalculatedtrips->push(trip);
    tp->add_task(new Task(TaxiCenter::wrapCreateRoute, this));
}

/******************************************************************************
* The function Operation: get a trip  and create a route from it's start and end
* points
******************************************************************************/
void TaxiCenter::createRoute(){
    pthread_mutex_lock(&lock);
    Trip* trip = this->uncalculatedtrips->front();
    this->uncalculatedtrips->pop();
//    cout << "trip num: " << trip->getId() << " is calcaulated" << endl;
    pthread_mutex_unlock(&lock);
    Point* start = trip->getStart();
    Point* end = trip->getEnd();
    vector<Point *> *route = map->getRoute(start, end);
    trip->setRoute(route);
//    cout << "trip num: " << trip->getId() << " ended calcaulating" << endl;
    delete route;
    delete end;
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
    // if driver id doesn't exist
    return NULL;
}

/******************************************************************************
* The function Operation: ask for driver location and retrun it
******************************************************************************/
vector<Point*>* TaxiCenter::getAllDriversLocations() {
    vector<Point*>* locations = new vector<Point*>();
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(SLEEP);
    }
    can_continue = false;
    for(int i = 0; i < numOfDrivers; i++){
        drivers->at(i)->setNextMission(6);
    }
    can_continue = true;
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(SLEEP);
    }
    for(int i = 0; i < numOfDrivers; i++){
        locations->push_back(drivers->at(i)->getLocation());
    }
    // if driver id doesn't exist
    return locations;
}

/******************************************************************************
* The function Operation: send trip to the driver
******************************************************************************/
void TaxiCenter::addTripToDriver(int time){
    while(avaliableDrivers->size() != numOfDrivers){
        sleep(SLEEP);
    }
    // vector of all the trips that need to delete
    vector <unsigned long> tripToDelete;
    Trip* trip = NULL;
    for(unsigned long i = 0; i < this->trips->size(); i++) {
        trip = this->trips->at(i);
        // if the trip have no route
        if(trip->size() == 0){
            delete trip;
            tripToDelete.push_back(i);
            continue;
        }
        // if the time of the trip arrived
        if (time >= trip->getTime()) {
            while (trip->size() == -1) {
                sleep(SLEEP);
            }
            Point *trip_start = trip->getStart();
            Comunicator *driver = this->getClosestDriver(*trip_start);
            delete trip_start;
            if(driver != NULL) {
                driver->setTrip(trip);
                // send trip to driver
                driver->setNextMission(4);
                // get message that trip accepted
                tripToDelete.push_back(i);
            }
        }
    }
    // delete all trips that sent or not legal
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
void TaxiCenter::wrapCreateRoute(void* center){
    ((TaxiCenter*) center)->createRoute();
}