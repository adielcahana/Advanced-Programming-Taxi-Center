#include <unistd.h>
#include "Comunicator.h"
int curr_time = 0;
bool can_continue = true;
/******************************************************************************
* The function Operation: the first talk with the driver, create driver info
* and send to the driver the map and the taxi, manage all the comunication
* with the driver
******************************************************************************/
void Comunicator::talkWithDriver() {
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
                        this->avaliable = true;
                        nextMission = 0;
                        time = curr_time;
                    }
                    this->comunicaorListener->avaliableEvent(this);
                    break;
                case 6: // ask for location
                    this->send(6);
                    if (this->receive(6) == 0) {
                        this->location = Point::deserialize(this->buffer);
                        this->comunicaorListener->avaliableEvent(this);
                        nextMission = 0;
                    } else {
                        cout << "something  bad happend" << endl;
                    }
                    break;
                default: // send again
                    this->send(0);
            }
        }else{
            sleep(SLEEP);
        }
    }
    this->send(7); //send "finish"
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

/******************************************************************************
* The function Operation: Comunicator destructor
******************************************************************************/
Comunicator::~Comunicator() {
    delete ((TaxiCenterProtocol *) this->protocol);
    pthread_join(*thread, NULL);
    delete thread;
    delete driverInfo;
}

/******************************************************************************
* The function Operation: set a taxi that the comunicator send to the driver
******************************************************************************/
void Comunicator::setTaxi(Taxi* taxi){
    ((TaxiCenterProtocol *) this->protocol)->setTaxi(taxi);
    delete taxi;
}

/******************************************************************************
* The function Operation: set a trip that the comunicator send to the driver
******************************************************************************/
void Comunicator::setTrip(Trip* trip){
    ((TaxiCenterProtocol *) this->protocol)->setTrip(trip);
    this->avaliable = false;
    delete trip;
}

/******************************************************************************
* The function Operation: return driver id
******************************************************************************/
int Comunicator::getDriverId(){
    return this->driverInfo->getDriverId();
}

/******************************************************************************
* The function Operation: return driver's taxi id
******************************************************************************/
int Comunicator::getTaxiId(){
    return this->driverInfo->getTaxiId();
}

/******************************************************************************
* The function Operation: get a number of mission and set it to the comunicator
******************************************************************************/
void Comunicator::setNextMission(int mission){
    this->comunicaorListener->unavaliableEvent(this);
    this->nextMission = mission;
}

/******************************************************************************
* The function Operation: return if the driver avaliable
******************************************************************************/
bool Comunicator::isAvaliable(){
    return this->avaliable;
}

/******************************************************************************
* The function Operation: set given AvaliableListener to the comunicator
******************************************************************************/
void Comunicator::addAvaliableListener(AvaliableListener* al){
    this->comunicaorListener = al;
}

/******************************************************************************
* The function Operation: remove the AvaliableListener from the comunicator
******************************************************************************/
void Comunicator::removeAvaliableListener() {
    this->comunicaorListener = NULL;
}

/******************************************************************************
* The function Operation: return Point that equal to the driver location
******************************************************************************/
Point* Comunicator::getLocation(){
    // sleep until the the driver not return the location
    while(nextMission != 0){
        sleep(SLEEP);
    }
    Point* loc = this->location;
    this->location = NULL;
    return loc;
}

/******************************************************************************
* The function Operation: return the comunicator's pthread
******************************************************************************/
pthread_t* Comunicator::getThread(){
    return this->thread;
}

/******************************************************************************
* The function Operation: set given pthread to the comunicator
******************************************************************************/
void Comunicator::setThread(pthread_t* thread){
    this->thread = thread;
};

/******************************************************************************
* The function Operation: get the time when the driver got to its position
******************************************************************************/
int Comunicator::getTime(){
    return this->time;
};

/******************************************************************************
* The function Operation: static function that called to talkWithDriver
* function that the thread could run this function
******************************************************************************/
void* Comunicator::wrapTalkWithDriver(void* comunicator){
    ((Comunicator*) comunicator)->talkWithDriver();
    // finish the thread
    pthread_exit(NULL);
}