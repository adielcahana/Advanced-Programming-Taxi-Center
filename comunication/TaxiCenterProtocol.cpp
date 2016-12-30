#include "TaxiCenterProtocol.h"

TaxiCenterProtocol::~TaxiCenterProtocol() {}

int TaxiCenterProtocol::translate(char* msg, int scenario){
    cout << msg << endl;
    switch(scenario){
        case 1:
            if(strstr(msg, "hello, I am a new driver, my Id is")){
                return 1;
            }
            break;
        case 2:
            if (strcmp(msg, "ok, waiting for map") == 0){
                return 2;
            }
            break;
        case 3:
            if (strcmp(msg, "ok, waiting for taxi") == 0){
                return 3;
            }
            break;
        case 4:
            if(strcmp(msg, "ok i am waiting for a trip") == 0){
                return 4;
            }
            break;
        case 5:
            if(strcmp(msg, "ok i am waiting to move") == 0){
                return 6;
            }
            break;
        case 7:
            if(strcmp(msg, "done") == 0){
                return 7;
            }
            break;
        default:
            return 0;
    }
    return 0;
}

string TaxiCenterProtocol::createMsg(int numOfMsg){
    switch (numOfMsg) {
        case 0:
            return "send again";
        case 1:
            return "hello i am sending your map";
        case 2:
            return this->map;
        case 3:
            return this->taxi;
        case 4:
            return this->trip;
        case 5:
            return "time passed";
        case 6:
            return "send your location";
        case 7:
            return "done";
        default:
            return "send again";
    }
}

void TaxiCenterProtocol::setMap(Map* map) {
    this->map = map->serialize();
}

void TaxiCenterProtocol::setTaxi(Taxi* taxi) {
    this->taxi = taxi->serialize();
}

void TaxiCenterProtocol::setTrip(Trip* trip) {
    this->trip = trip->serialize();
}
