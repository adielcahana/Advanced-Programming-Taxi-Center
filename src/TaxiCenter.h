#ifndef ASS2_TAXICENTER_H
#define ASS2_TAXICENTER_H

#include "Trip.h"
#include "Comunicator.h"
#include <pthread.h>
#include "../comunication/Server.h"
#include "CommunicatorListener.h"

/******************************************************************************
* TaxiCenter: have information about all the drivers the cabs and the trip.
* know to connect between new trip to avaliable driver, also know the map.
******************************************************************************/

class TaxiCenter: public Server{
private:
    vector <Comunicator*>* avaliableDrivers;
    vector <Taxi*>* avaliableCabs;
    vector <Trip*>* trips;
    queue <Trip*>* uncalculatedtrips;
    CommunicatorListener* listener;
    Map* map;
    int numOfDrivers;
    pthread_mutex_t lock;
public:
    TaxiCenter(Protocol* protocol, Tcp* tcp, Map* map);
    ~TaxiCenter();
    void acceptNewDriver();
    void addComunicator(Comunicator* comunicator);
    void addAvaliableTaxi(Taxi *taxi);
    Comunicator* getClosestDriver(Point location);
    Taxi* getTaxiById(int id);
    void addTrip(Trip* trip);
    void createRoute();
    void timePassed();
//    DriverInfo * findClosestDriver(Point start);
    Point *getDriverLocation(int driverId);
    void addTripToDriver(int time);
    void sendFinish();
    static void* wrapCreateRoute(void* center);
};

#endif //ASS2_TAXICENTER_H
