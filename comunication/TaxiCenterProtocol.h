#ifndef EX4_SERVERPROTOCOL_H
#define EX4_SERVERPROTOCOL_H

#include <iostream>
#include "Protocol.h"
#include <string>
#include "../src/Taxi.h"

using namespace std;

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
    ~TaxiCenterProtocol();
    string createMsg(int numOfMsg);
    int translate(char* msg, int scenario);
};

#endif //EX4_SERVERPROTOCOL_H
