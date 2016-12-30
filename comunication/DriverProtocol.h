//
// Created by adi on 26/12/16.
//

#ifndef EX4_DRIVERPROTOCOL_H
#define EX4_DRIVERPROTOCOL_H

#include "Protocol.h"
#include "../src/Map.h"
#include <string.h>
#include <iostream>

using namespace std;
class DriverProtocol: public Protocol {
private:
    int driverId;
    int taxiId;
    Point** location;
public:
    DriverProtocol(int id, int taxiId, Point** location): driverId(driverId), taxiId(taxiId), location(location){};
    virtual int translate(char* msg, int scenario);
    virtual string createMsg(int numOfMsg);
};

#endif //EX4_DRIVERPROTOCOL_H
