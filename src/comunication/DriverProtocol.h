//
// Created by adi on 26/12/16.
//

#ifndef EX4_DRIVERPROTOCOL_H
#define EX4_DRIVERPROTOCOL_H

#include "Protocol.h"
#include "../Navigation/Point.h"
#include <string.h>
#include <iostream>

using namespace std;

/******************************************************************************
* DriverProtocol: the operation protocol of the driver as a client, use
* scenarios for sending messages and translate given messages, inherit from
* protocol interface
******************************************************************************/
class DriverProtocol: public Protocol {
private:
    int driverId;
    int taxiId;
    Point** location;
public:
    DriverProtocol(int id, int taxiId, Point** location): driverId(id), taxiId(taxiId), location(location){};
    virtual int translate(char* msg, int scenario);
    virtual string createMsg(int numOfMsg);
};

#endif //EX4_DRIVERPROTOCOL_H
