#include <unistd.h>
#include "Comunicator.h"

bool can_continue = true;
/******************************************************************************
* The function Operation: the first talk with the driver, create driver info
* and send to the driver the map and the taxi
******************************************************************************/
void Comunicator::talkWithDriver() {
    DriverInfo* driverInfo = NULL;
    while (nextMission != 7) {
        if (nextMission == 0) {
            sleep(SLEEP);
        } else if (can_continue == true) {
            switch (nextMission) {
                case 1:
                    // first message from driver
                    nextMission = this->receive(1);
                    // create driver info
                    this->driverInfo = this->createDriverInfo(this->buffer);
                    // send hello to driver
                    this->send(1);
                    nextMission = this->receive(2);
                    break;
                case 2:
                    // send map to the driver
                    this->send(2);
                    nextMission = this->receive(3);
                    this->comunicaorListener->avaliableEvent(this);
                    break;
                case 3:
                    // send taxi to driver
                    this->send(3);
                    // verify that taxi accepted
                    nextMission = this->receive(4);
                    this->comunicaorListener->avaliableEvent(this);
                    break;
                case 4:
                    // send trip to driver
                    this->avaliable = false;
                    this->send(4);
                    nextMission = this->receive(5);
                    this->comunicaorListener->avaliableEvent(this);
                    break;
                case 5: // tell the driver that time passed (time++)
                    this->send(5);
                    nextMission = this->receive(7);
                    if (nextMission == 7) {
                        //todo: notify that driver is free for new trip
                        nextMission = 0;
                        this->avaliable = true;
                    }
                    this->comunicaorListener->avaliableEvent(this);
                    break;
                case 6: // ask for location
                    this->send(6);
                    nextMission = this->receive(6);
                    this->location = Point::deserialize(this->buffer);
                    this->comunicaorListener->avaliableEvent(this);
                    break;
                default: // send again
                    this->send(0);
            }
        }else{
            sleep(SLEEP);
        }
//        if (nextMission != 0){
//            ++nextMission;
//        }
    }
    this->send(7);
    this->comunicaorListener->avaliableEvent(this);
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

Comunicator::~Comunicator() {
    delete ((TaxiCenterProtocol *) this->protocol);
    delete thread;
    delete driverInfo;
}

void Comunicator::setTaxi(Taxi* taxi){
    ((TaxiCenterProtocol *) this->protocol)->setTaxi(taxi);
    delete taxi;
}

void Comunicator::setTrip(Trip* trip){
    ((TaxiCenterProtocol *) this->protocol)->setTrip(trip);
    this->avaliable = false;
    delete trip;
}

int Comunicator::getDriverId(){
    return this->driverInfo->getDriverId();
}

int Comunicator::getTaxiId(){
    return this->driverInfo->getTaxiId();
}

void Comunicator::setNextMission(int mission){
    this->comunicaorListener->unavaliableEvent(this);
    this->nextMission = mission;
}

bool Comunicator::isAvaliable(){
    return this->avaliable;
}

void Comunicator::addAvaliableListener(AvaliableListener* al){
    this->comunicaorListener = al;
}
void Comunicator::removeAvaliableListener(AvaliableListener* al){
    this->comunicaorListener = NULL;
}

Point* Comunicator::getLocation(){
    setNextMission(6);
    while(nextMission != 0){
        sleep(SLEEP);
    }
    Point* loc = this->location;
    this->location = NULL;
    return loc;
}

pthread_t* Comunicator::getThread(){
    return this->thread;
}

void Comunicator::setThread(pthread_t* thread){
    this->thread = thread;
};


void* Comunicator::wrapTalkWithDriver(void* comunicator){
    ((Comunicator*) comunicator)->talkWithDriver();
    pthread_exit(NULL);
}