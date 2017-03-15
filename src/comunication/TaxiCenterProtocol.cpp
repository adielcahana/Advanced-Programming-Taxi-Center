#include "TaxiCenterProtocol.h"

/******************************************************************************
* The Function Operation: this map = given map serialization
******************************************************************************/
void TaxiCenterProtocol::setMap(Map* map) {
    this->map = map->toString();
}

/******************************************************************************
* The Function Operation: this taxi = given taxi serialization
******************************************************************************/
void TaxiCenterProtocol::setTaxi(Taxi* taxi) {
    this->taxi = taxi->toString();
}

/******************************************************************************
* The Function Operation: this trip = given trip serialization
******************************************************************************/
void TaxiCenterProtocol::setTrip(Trip* trip) {
    this->trip = trip->toString();
}

/******************************************************************************
* The Function Operation: get message and excpected scenario and return
* the right scenario for the massege
******************************************************************************/
int TaxiCenterProtocol::translate(char* msg, int scenario){
    switch(scenario){
        case 1: // first massege from the driver
            if(strstr(msg, "hello, I am a new driver, my Id is")){
                return 1;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 2: // the driver wait for map
            if (strcmp(msg, "ok, waiting for map") == 0){
                return 2;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 3: // the driver wait for taxi
            if (strcmp(msg, "ok, waiting for taxi") == 0){
                return 0;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 4: // the driver get taxi (wait for trip)
            if(strcmp(msg, "taxi accepted") == 0){
                return 0;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 5: // the driver get a trip
            if(strcmp(msg, "trip accepted") == 0){
                return 0;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 6: // the driver send his location
            if (strstr(msg, "point")) {
                string str = string(msg);
                str.erase(0, 7);
                strcpy(msg, str.c_str());
                return 0;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 7: // the driver return that he got that time passed
            if (strcmp(msg, "time passed") == 0) {
                return 0;
            } else if(strcmp(msg, "done") == 0){
                // if the driver finish his trip
                return 7;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        default:
            return 0;
    }
    return 0;
}

/******************************************************************************
* The Function Operation: get number of message and create the right message
******************************************************************************/
string TaxiCenterProtocol::createMsg(int numOfMsg){
    switch (numOfMsg) {
        case 0:
            return "send again";
        case 1:
            return "hello I am sending your map";
        case 2: // send the string of the map
            return "map: " + this->map;
        case 3: // send the string of the taxi
            return "taxi: " + this->taxi;
        case 4: // send the string of the trip
            return "trip: " + this->trip;
        case 5: // tell the driver that time passed (time++)
            return "time passed";
        case 6: // ask for location
            return "send your location";
        case 7: // return to driver that got that he finish trip
            return "finish";
        default:
            return "send again";
    }
}
