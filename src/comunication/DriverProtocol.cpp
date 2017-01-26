#include <sstream>
#include "DriverProtocol.h"

using namespace std;

/******************************************************************************
* The Function Operation: get message and excpected scenario and return
* the right scenario for the massege
******************************************************************************/
int DriverProtocol::translate(char* msg, int scenario){
    string str;
    // if the server program end
    if(strcmp(msg, "finish") == 0){
        return 9;
    }
    switch(scenario) {
        case 1: // first massege from the taxi center
            if (strcmp(msg, "hello I am sending your map") == 0) {
                return 2;
            }
            else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 2: // get map from the taxi center
            if (strstr(msg, "map")) {
                // delete start of the map message
                str = string(msg);
                str.erase(0,5);
                strcpy(msg, str.c_str());
                return 3;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 3: // get taxi from the taxi center
            if (strstr(msg, "taxi")) {
                // delete start of the taxi message
                str = string(msg);
                str.erase(0,6);
                strcpy(msg, str.c_str());
                return 4;
            } else if (strcmp(msg, "time passed") == 0) {
                // if the taxi center said that time passed
                return 6;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 4:
            if (strstr(msg, "trip:")) {
                // delete start of the trip message
                str = string(msg);
                str.erase(0,6);
                strcpy(msg, str.c_str());
                return 5;
            } else if (strcmp(msg, "time passed") == 0) {
                // if the taxi center said that time passed
                return 6;
            } else if(strcmp(msg, "send your location") == 0) {
                // if the taxi center ask for location
                return 7;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
        case 5:
            if (strcmp(msg, "time passed") == 0) {
                // if the taxi center said that time passed
                return 6;
            } else if(strcmp(msg, "send your location") == 0){
                // if the taxi center ask for location
                return 7;
            } else if(strcmp(msg, "send again") == 0){
                return scenario;
            }
            break;
//        case 6:
//            if (strcmp(msg, "done") == 0) {
//                // the taxi center got message that driver finish
//                return 8;
//            }
        default:
            return 0;
    }
    return 0;
}

/******************************************************************************
* The Function Operation: get number of message and create the right message
******************************************************************************/
string DriverProtocol::createMsg(int numOfMsg){
    stringstream driver_id, taxi_id;
    driver_id << driverId;
    taxi_id << taxiId;
    switch (numOfMsg){
        case 0:
            return "send again";
        case 1: // first message to taxi center
            return "hello, I am a new driver, my Id is:" + driver_id.str() +
                    " my taxi id is:" + taxi_id.str();
        case 2: // wait for map
            return "ok, waiting for map";
        case 3: // wait for taxi
            return "ok, waiting for taxi";
        case 4: // got map (wait for trip)
            return "taxi accepted";
        case 5: // got trip
            return "trip accepted";
        case 6: // got that time passed
            return "time passed";
        case 7: // send the location
            return "point: " + (*location)->toString();
        case 8: // send that finish the trip
            return "done";
        default:
            break;
    }
}

