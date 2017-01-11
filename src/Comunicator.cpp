#include "Comunicator.h"

/******************************************************************************
* The function Operation: the first talk with the driver, create driver info
* and send to the driver the map and the taxi
******************************************************************************/
void Comunicator::talkWithDriver(Map* map, Taxi* taxi) {
    int operation = 0;
    DriverInfo* driverInfo = NULL;
    do {
        // first message from driver
        operation = this->receive(++operation);
//        cout << operation << endl;
        switch (operation){
            case 1:
                // create driver info
                this->driverInfo = this->createDriverInfo(this->buffer);
                // send hello to driver
                this->send(1);
                break;
            case 2:
                // send map to the driver
                this->setProtocolMap(map);
                this->send(2);
                break;
            case 3:
                // send taxi to driver
                this->setProtocolTaxi(taxi);
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
* The function Operation: get string of Driver info and return Driver info
******************************************************************************/
DriverInfo* Comunicator::createDriverInfo(string buffer) {
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
* The function Operation: take the map and set it in the protocol
******************************************************************************/
void Comunicator::setProtocolMap(Map* map) {
    ((TaxiCenterProtocol *) this->protocol)->setMap(map);
}

/******************************************************************************
* The function Operation: get a taxi and set it in the protocol
******************************************************************************/
void Comunicator::setProtocolTaxi(Taxi *taxi) {
    ((TaxiCenterProtocol *) this->protocol)->setTaxi(taxi);
}

Comunicator::~Comunicator() {
    delete ((TaxiCenterProtocol *) this->protocol);
}

/******************************************************************************
* The function Operation: get a trip and set it in the protocol
******************************************************************************/
void Comunicator::setProtocolTrip(Trip *trip) {
    ((TaxiCenterProtocol *) this->protocol)->setTrip(trip);
}
