#ifndef EX4_SERVERPROTOCOL_H
#define EX4_SERVERPROTOCOL_H

#include <iostream>
#include "Protocol.h"
#include <string>
#include "../src/Taxi.h"

using namespace std;

/******************************************************************************
* TaxiCenterProtocol: the operation protocol of the taxi center as a server,
* use scenarios for sending messages and translate given messages, inherit from
* protocol interface
******************************************************************************/
class TaxiCenterProtocol: public Protocol{
private:
    string map;
    string taxi;
    string trip;
public:
    void setMap(Map* map);
    void setTaxi(Taxi* taxi);
    void setTrip(Trip* trip);
    TaxiCenterProtocol(): Protocol(){};
    string createMsg(int numOfMsg);
    int translate(char* msg, int scenario);
};

#endif //EX4_SERVERPROTOCOL_H
