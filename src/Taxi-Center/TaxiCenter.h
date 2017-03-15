#ifndef ASS2_TAXICENTER_H
#define ASS2_TAXICENTER_H

#include <pthread.h>
#include <unistd.h>
#include <climits>
#include "../Navigation/Trip.h"
#include "Comunicator.h"
#include "../comunication/Server.h"
#include "CommunicatorListener.h"
#include "../general/ThreadPool.h"
/******************************************************************************
* TaxiCenter: have information about all the drivers the cabs and the trip.
* know to connect between new trip to avaliable driver, also know the map.
******************************************************************************/

class TaxiCenter: public Server{
private:
    vector <Comunicator*>* drivers;
    vector <Comunicator*>* avaliableDrivers;
    vector <Taxi*>* avaliableCabs;
    vector <Trip*>* trips;
    queue <Trip*>* uncalculatedtrips;
    CommunicatorListener* listener;
    Map* map;
    int numOfDrivers;
    ThreadPool* tp;
    pthread_mutex_t lock;
    void createRoute();
    static void wrapCreateRoute(void* center);
public:
    TaxiCenter(Protocol* protocol, Tcp* tcp, Map* map);
    ~TaxiCenter();
    void acceptNewDriver();
    void addComunicator(Comunicator* comunicator);
    void addAvaliableTaxi(Taxi *taxi);
    Comunicator* getClosestDriver(Point location);
    Taxi* getTaxiById(int id);
    void addTrip(Trip* trip);
    void timePassed();
    Point *getDriverLocation(int driverId);
    vector<Point*>* getAllDriversLocations();
    void addTripToDriver(int time);
    void sendFinish();
};

#endif //ASS2_TAXICENTER_H
