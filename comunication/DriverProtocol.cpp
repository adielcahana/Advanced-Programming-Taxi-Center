#include <sstream>
#include "DriverProtocol.h"
using namespace std;
int DriverProtocol::translate(char* msg, int scenario){
    string str;
    switch(scenario) {
        case 1:
            if (strcmp(msg, "hello I am sending your map") == 0) {
                return 2;
            }
            break;
        case 2:
            if (strstr(msg, "map")) {
                str = string(msg);
                str.erase(0,5);
                strcpy(msg, str.c_str());
                return 3;
            }
            break;
        case 3:
            if (strstr(msg, "taxi")) {
                str = string(msg);
                str.erase(0,6);
                strcpy(msg, str.c_str());
                return 4;
            } else if (strcmp(msg, "time passed") == 0) {
                return 6;
            }
            break;
        case 4:
            if (strstr(msg, "trip")) {
                str = string(msg);
                str.erase(0,6);
                strcpy(msg, str.c_str());
                return 5;
            } else if (strcmp(msg, "time passed") == 0) {
                return 6;
            } else if(strcmp(msg, "send your location") == 0){
                return 7;
            }
            break;
        case 5:
            if (strcmp(msg, "time passed") == 0) {
                return 6;
            }
            else if(strcmp(msg, "send your location") == 0){
                return 7;
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
            return "taxi accepted";
        case 5:
            return "trip accepted";
        case 6:
            return "time passed";
        case 7:
            return "point: " + string((*location)->serialize());
        case 8:
            return "done";
        default:
            break;
    }
}

