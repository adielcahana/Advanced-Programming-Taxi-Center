#ifndef EX4_SERVERPROTOCOL_H
#define EX4_SERVERPROTOCOL_H

#include <iostream>
#include <string>
#include "Protocol.h"
#include "../Driver/Taxi.h"
#include "../Navigation/Map.h"
#include "../Navigation/Trip.h"

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
    ~TaxiCenterProtocol(){};
    string createMsg(int numOfMsg);
    int translate(char* msg, int scenario);
};

#endif //EX4_SERVERPROTOCOL_H
