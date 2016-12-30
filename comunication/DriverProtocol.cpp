#include <sstream>
#include "DriverProtocol.h"
using namespace std;
int DriverProtocol::translate(char* msg, int scenario){
    switch(scenario) {
        case 1:
            if (strcmp(msg, "hello I am sending your map") == 0) {
                return 2;
            }
            break;
        case 5:
            if (strcmp(msg, "time passed") == 0) {
                return 5;
            }
            else if(strcmp(msg, "send your location") == 0){
                    return 6;
            }
            break;
        default:
            return 0;
    }
    return 0;
}

string DriverProtocol::createMsg(int numOfMsg){
    stringstream str, str1;
    str << this->driverId;
    string id = str.str();
    str1 << this->taxiId;
    string taxi = str1.str();
    switch (numOfMsg){
        case 0:
            return "send again";
        case 1:
            return "hello, I am a new driver, my Id is:" + id + " my taxi id is:" + taxi;
        case 2:
            return "ok, waiting for map";
        case 3:
            return "ok, waiting for taxi";
        case 4:
            return "ok, waiting to trip";
        case 5:
            return "ok, waiting for time passed";
        case 6:
            return (*location)->serialize();
        case 7:
            return "done";
        default:
            break;
    }
}

