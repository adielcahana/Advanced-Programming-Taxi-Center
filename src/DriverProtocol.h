//
// Created by adi on 26/12/16.
//

#ifndef EX4_DRIVERPROTOCOL_H
#define EX4_DRIVERPROTOCOL_H

#include "Protocol.h"
#include "Map.h"
#include <string.h>
#include <iostream>

using namespace std;
class DriverProtocol: public Protocol {
private:
    int id;
    Point** location;
public:
    DriverProtocol(int id, Point** location): id(id), location(location){};
    virtual int translate(char* msg, int scenario);
    virtual const char* createMsg(int numOfMsg);
};

#endif //EX4_DRIVERPROTOCOL_H
