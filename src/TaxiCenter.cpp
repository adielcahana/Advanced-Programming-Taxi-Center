#include "TaxiCenter.h"

/******************************************************************************
* The function Operation: TaxiCenter constructor.
******************************************************************************/
TaxiCenter::TaxiCenter(Protocol *protocol, Udp *udp, Map *map) : Server(protocol, udp) {
    drivers = new vector <DriverInfo*>;
    avaliableDrivers = new vector <DriverInfo*>;
    avaliableCabs = new vector <Taxi*>();
    trips = new vector <Trip*>();
    this->map = map;
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
}

/******************************************************************************
* The function Operation: get a driver and add him to the driver's vector
* give the driver a cab by his taxi id
******************************************************************************/
void TaxiCenter::addDriverInfo(DriverInfo* driverInfo){
    this->drivers->push_back(driverInfo);
    this->avaliableDrivers->push_back(driverInfo);
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
* The function Operation: get a trip create a route from it's start and end
* points and check if some driver can get this trip
******************************************************************************/
void TaxiCenter::addTrip(Trip* trip){
    this->trips->push_back(trip);
}

/******************************************************************************
* The function Operation: take the map and set it in the protocol
******************************************************************************/
void TaxiCenter::setProtocolMap() {
    ((TaxiCenterProtocol *) this->protocol)->setMap(this->map);
}

/******************************************************************************
* The function Operation: get a taxi and set it in the protocol
******************************************************************************/
void TaxiCenter::setProtocolTaxi(int taxiId) {
    for(int i = 0; i < this->avaliableCabs->size(); i++){
        // check for the right taxi
        if(taxiId == this->avaliableCabs->at(i)->getId()){
            ((TaxiCenterProtocol *) this->protocol)->setTaxi(this->avaliableCabs->at(i));
            // taxi no longer avaliable
            delete this->avaliableCabs->at(i);
            this->avaliableCabs->erase(avaliableCabs->begin() + i);
            return;
        }
    }
}

/******************************************************************************
* The function Operation: get a trip and set it in the protocol
******************************************************************************/
void TaxiCenter::setProtocolTrip(Trip* trip) {
            ((TaxiCenterProtocol *) this->protocol)->setTrip(trip);
}

/******************************************************************************
* The function Operation: notify all the driver that time passed
******************************************************************************/
void TaxiCenter::timePassed(){
    int operation;
    for(int i = 0; i < drivers->size(); i++){
        this->send(5);
        operation = this->receive(7);
        if(operation == 7){
            DriverInfo* driverInfo = drivers->at(i);
            this->avaliableDrivers->push_back(driverInfo);
            this->send(7);
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
Point * TaxiCenter::getDriverLocation() {
    this->send(6);
    while (this->receive(6) != 6){
        this->send(0);
    }
    return Point::deserialize(this->buffer);
}

/******************************************************************************
* The function Operation: get string of Driver info and return Driver info
******************************************************************************/
DriverInfo *TaxiCenter::createDriverInfo(string buffer) {
    char *c = new char[buffer.length() + 1];
    strcpy(c, buffer.c_str());
    string str = strtok(c, ":");
    // get the driver id
    int driverId  = atoi(strtok(NULL, " "));
    string str1 = strtok(NULL, ":");
    // get the taxi id
    int taxiId = atoi(strtok(NULL, " "));
    delete[](c);
    return new DriverInfo(driverId, taxiId);
}

/******************************************************************************
* The function Operation: the first talk with the driver, create driver info
* and send to the driver the map and the taxi
******************************************************************************/
void TaxiCenter::talkWithDriver() {
    int operation = 0;
    DriverInfo* driverInfo = NULL;
    do {
        // first message from driver
        operation = this->receive(++operation);
//        cout << operation << endl;
        switch (operation){
            case 1:
                // create driver info
                driverInfo = this->createDriverInfo(this->buffer);
                this->addDriverInfo(driverInfo);
                // send hello to driver
                this->send(1);
                break;
            case 2:
                // send map to the driver
                this->setProtocolMap();
                this->send(2);
                break;
            case 3:
                // send taxi to driver
                this->setProtocolTaxi(driverInfo->getTaxiId());
                this->send(3);
                break;
            case 4:
                break;
            default:
                this->send(0);
        }
    }while(operation < 4);
}

/******************************************************************************
* The function Operation: send trip to the driver
******************************************************************************/
void TaxiCenter::addTripToDriver(int time){
    for(int i = 0; i < this->trips->size(); i++){
        // if the time of the trip arrived
        if (time >= this->trips->at(i)->time){
            this->setProtocolTrip(this->trips->at(i));
            // send trip to driver
            this->send(4);
            // get message that trip accepted
            this->receive(5);
            // delete the trip from the taxi center list
            delete this->trips->at(i);
            this->trips->erase(trips->begin() + i);
            // driver is no longer avaliable
            this->avaliableDrivers->pop_back();
        }
    }
}
