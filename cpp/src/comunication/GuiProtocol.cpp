//
// Created by adi on 04/02/17.
//

#include "GuiProtocol.h"

string GuiProtocol::createMsg(int numOfMsg) {
    stringstream locations_srl;
    Point* location;
    switch (numOfMsg){
        case -1:
            return "error\n";
        case 0:
            return "accepted\n";
        case 1: // send the game map to the gui
            return this->map + "\n";
        case 2: //send the gui all the new locations
            for(int i = 0; i < locations->size(); i++){
                location = locations->at(i);
                locations_srl << location->toString();
                delete location;
                locations_srl << " ";
            }
            delete locations;
            locations = NULL;
            return locations_srl.str() + "\n";
        default:
            break;
    }
}

int GuiProtocol::translate(char *msg, int scenario) {
    string str;
    if (strstr(msg, "num of drivers:") != NULL) {
        string str = string(msg);
        str.erase(0, 15);
        strcpy(msg, str.c_str());
        return 1;
    }
    else if (strstr(msg, "taxi") != NULL) {
        // delete start of the taxi message
        str = string(msg);
        str.erase(0,6);
        strcpy(msg, str.c_str() );
        return 3;
    }
    else if (strstr(msg, "trip:") != NULL) {
        // delete start of the trip message
        str = string(msg);
        str.erase(0,6);
        strcpy(msg, str.c_str());
        return 2;
    }
    else if (strstr(msg, "end") != NULL) {
        return 7;
    }
    else if (strstr(msg, "time passed") != NULL) {
        return 9;
    }
    return 0;
}

/******************************************************************************
* The Function Operation: this map = given map serialization
******************************************************************************/
void GuiProtocol::setMap(Map* map) {
    this->map = map->toString();
}

/******************************************************************************
* The Function Operation: this locations = given locations of all drivers
******************************************************************************/
void GuiProtocol::setLocations(vector<Point *> *locations) {
    this->locations = locations;
}

GuiProtocol::~GuiProtocol() {
    if (locations != NULL) {
        for (int i=0; i<locations->size(); i++){
            delete locations->at(i);
        }
        delete locations;
    }
}