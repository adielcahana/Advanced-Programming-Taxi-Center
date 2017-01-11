#include "TaxiCenter.h"

/******************************************************************************
* The function Operation: TaxiCenter constructor.
******************************************************************************/
TaxiCenter::TaxiCenter(Protocol *protocol, Tcp *tcp, Map *map) : Server(protocol, tcp) {
    drivers = new vector <Comunicator*>;
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
    for(int i = 0; i < this->drivers->size(); i++){
        delete drivers->at(i);
    }
    delete trips;
    delete drivers;
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
* The function Operation: add a trip to the trip list
******************************************************************************/
void TaxiCenter::addTrip(Trip* trip){
    this->trips->push_back(trip);
    this->uncalculatedtrips->push(trip);
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


//void TaxiCenter::setProtocolTaxi(int taxiId) {
//    for(int i = 0; i < this->avaliableCabs->size(); i++){
//        // check for the right taxi
//        if(taxiId == this->avaliableCabs->at(i)->getId()){
//            ((TaxiCenterProtocol *) this->protocol)->setTaxi(this->avaliableCabs->at(i));
//            // taxi no longer avaliable
//            delete this->avaliableCabs->at(i);
//            this->avaliableCabs->erase(avaliableCabs->begin() + i);
//            return;
//        }
//    }
//}


/******************************************************************************
* The function Operation: notify all the driver that time passed
******************************************************************************/
void TaxiCenter::timePassed(){
    int operation;
    for(int i = 0; i < drivers->size(); i++){
        drivers->at(i)->send(5);
        operation = drivers->at(0)->receive(7);
        if(operation == 7){
            Comunicator* comunicator = drivers->at(i);
            this->avaliableDrivers->push_back(comunicator);
            drivers->at(0)->send(7);
        }
    }
}

/******************************************************************************
* The function Operation: check if all the driver is avaliable
******************************************************************************/
bool TaxiCenter::shouldStop(){
    return (avaliableDrivers->size() == drivers->size());
}

/******************************************************************************
* The function Operation: ask for driver location and retrun it
******************************************************************************/
Point* TaxiCenter::getDriverLocation() {
    drivers->at(0)->send(6);
    while (drivers->at(0)->receive(6) != 6){
        drivers->at(0)->send(0);
    }
    return Point::deserialize(drivers->at(0)->buffer);
}

/******************************************************************************
* The function Operation: send trip to the driver
******************************************************************************/
void TaxiCenter::addTripToDriver(int time){
    Trip* trip = NULL;
    for(unsigned long i = 0; i < this->trips->size(); i++){
        // if the time of the trip arrived
        trip = this->trips->at(i);
        if (time >= trip->time){
            if (trip->route == NULL){
                pthread_t* thread = trip->getThread();
                pthread_join(*thread, NULL);
                delete thread;
                trip->setThread(NULL);
            }
            drivers->at(0)->setProtocolTrip(trip);
            // send trip to driver
            drivers->at(0)->send(4);
            // get message that trip accepted
            drivers->at(0)->receive(5);
            // delete the trip from the taxi center list
            delete this->trips->at(i);
            this->trips->erase(trips->begin() + i);
            // driver is no longer avaliable
            this->avaliableDrivers->pop_back();
        }
    }
}

void TaxiCenter::addComunicator(Comunicator *comunicator) {
    this->drivers->push_back(comunicator);
    this->avaliableDrivers->push_back(comunicator);
}

void TaxiCenter::acceptNewDriver() {
    Tcp* tcp = this->accept();
    Taxi* taxi = this->avaliableCabs->at(0);
    Comunicator* comunicator = new Comunicator(new TaxiCenterProtocol(), tcp);
    this->addComunicator(comunicator);
    comunicator->talkWithDriver(this->map, taxi);
}

void TaxiCenter::sendFinish() {
    drivers->at(0)->send(8);
}
