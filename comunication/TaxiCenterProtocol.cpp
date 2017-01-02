#include "TaxiCenterProtocol.h"

TaxiCenterProtocol::~TaxiCenterProtocol() {}

int TaxiCenterProtocol::translate(char* msg, int scenario){
    switch(scenario){
        case 1:
            if(strstr(msg, "hello, I am a new driver, my Id is")){
                return 1;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 2:
            if (strcmp(msg, "ok, waiting for map") == 0){
                return 2;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 3:
            if (strcmp(msg, "ok, waiting for taxi") == 0){
                return 3;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 4:
            if(strcmp(msg, "taxi accepted") == 0){
                return 4;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 5:
            if(strcmp(msg, "trip accepted") == 0){
                return 5;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 6:
            if (strstr(msg, "point")) {
                string str = string(msg);
                str.erase(0, 7);
                strcpy(msg, str.c_str());
                return 6;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 7:
            if (strcmp(msg, "time passed") == 0) {
                return 5;
            } else if(strcmp(msg, "done") == 0){
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

string TaxiCenterProtocol::createMsg(int numOfMsg){
    switch (numOfMsg) {
        case 0:
            return "send again";
        case 1:
            return "hello I am sending your map";
        case 2:
            return "map: " + this->map;
        case 3:
            return "taxi: " + this->taxi;
        case 4:
            return "trip: " + this->trip;
        case 5:
            return "time passed";
        case 6:
            return "send your location";
        case 7:
            return "done";
        case 8:
            return "finish";
        default:
            return "send again";
    }
}

void TaxiCenterProtocol::setMap(Map* map) {
    this->map = map->toString();
}

void TaxiCenterProtocol::setTaxi(Taxi* taxi) {
    this->taxi = taxi->serialize();
}

void TaxiCenterProtocol::setTrip(Trip* trip) {
    this->trip = trip->serialize();
}
