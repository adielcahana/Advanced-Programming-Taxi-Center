#ifndef ASS2_TAXICENTER_H
#define ASS2_TAXICENTER_H

#include "Trip.h"
#include "DriverInfo.h"
#include "../comunication/Server.h"

/******************************************************************************
* TaxiCenter: have information about all the drivers the cabs and the trip.
* know to connect between new trip to avaliable driver, also know the map.
******************************************************************************/
class TaxiCenter: public Server{
private:
    vector <DriverInfo*>* drivers;
    vector <DriverInfo*>* avaliableDrivers;
    vector <Taxi*>* avaliableCabs;
    vector <Trip*>* trips;
    Map* map;
public:
    TaxiCenter(Protocol* protocol, Udp* udp, Map* map);
    ~TaxiCenter();
    void addDriverInfo(DriverInfo* driverInfo);
    void addAvaliableTaxi(Taxi *taxi);
    DriverInfo* createDriverInfo(string buffer);
    Taxi* searchTaxiById(int id);
    void addTrip(Trip* trip);
    void timePassed();
//    DriverInfo * findClosestDriver(Point start);
    void setProtocolMap();
    void setProtocolTaxi(int taxiId);
    void setProtocolTrip(Trip* trip);
    Point *getDriverLocation();
    bool shouldStop();
    void talkWithDriver();
    void addTripToDriver(int time);
};

#endif //ASS2_TAXICENTER_H
