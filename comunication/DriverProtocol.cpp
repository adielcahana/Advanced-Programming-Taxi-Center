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
        case 4:
            if (strstr(msg, "advance, time is")) {
                return 5;
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
            return "hello, I am a new driver, my Id is:0 my taxi id is:0 ";
        case 2:
            return "ok, waiting for map";
        case 3:
            return "ok, waiting for taxi";
        case 4:
            return "ok, waiting for go";
        case 5:
            return (*location)->serialize();
        case 6:
            return "done";
        default:
            break;
    }
}

