#include <sstream>
#include "DriverProtocol.h"
using namespace std;
int DriverProtocol::translate(char* msg, int scenario){
    string str;
    if(strcmp(msg, "finish") == 0){
        return 9;
    }
    switch(scenario) {
        case 1:
            if (strcmp(msg, "hello I am sending your map") == 0) {
                return 2;
            }
            else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 2:
            if (strstr(msg, "map")) {
                str = string(msg);
                str.erase(0,5);
                strcpy(msg, str.c_str());
                return 3;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
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
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 4:
            if (strstr(msg, "trip:")) {
                str = string(msg);
                str.erase(0,6);
                strcpy(msg, str.c_str());
                return 5;
            } else if (strcmp(msg, "time passed") == 0) {
                return 6;
            } else if(strcmp(msg, "send your location") == 0) {
                return 7;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 5:
            if (strcmp(msg, "time passed") == 0) {
                return 6;
            } else if(strcmp(msg, "send your location") == 0){
                return 7;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 6:
            if (strcmp(msg, "done") == 0) {
                return 8;
            }
        default:
            return 0;
    }
    return 0;
}

string DriverProtocol::createMsg(int numOfMsg){
    stringstream str, str1;
    str << this->driverId;
    str1 << this->taxiId;
    switch (numOfMsg){
        case 0:
            return "send again";
        case 1:
            return "hello, I am a new driver, my Id is:" + str.str() + " my taxi id is:" + str.str();
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
            return "point: " + (*location)->toSting();
        case 8:
            return "done";
        default:
            break;
    }
}

