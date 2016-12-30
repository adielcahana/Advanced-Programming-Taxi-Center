//
// Created by adi on 26/12/16.
//

#include <sstream>
#include "DriverProtocol.h"
using namespace std;
int DriverProtocol::translate(char* msg, int scenario){
    switch(scenario) {
        case 1:
            if (strcmp(msg, "hello i am sending your map") == 0) {
                return 2;
            }
            break;
        case 4:
            if (strcmp(msg, "time passed") == 0) {
                return 5;
            } else if (strcmp(msg, "send your location") == 0) {
                return 6;
            }
            break;

        default:
            return 0;
    }
    return 0;
}

string DriverProtocol::createMsg(int numOfMsg){
    switch (numOfMsg){
        case 0:
            return "send again";
        case 1:
            return "hello, I am a new driver, my Id is:10 my taxi id is:10 ";
        case 2:
            return "ok, waiting for map";
        case 3:
            return "ok, waiting for taxi";
        case 4:
            return "ok, waiting for trip";
        case 5:
            return "ok, waiting to move";
        case 6:
            return (*location)->serialize();
        case 7:
            return "done";
        default:
            break;
    }
}