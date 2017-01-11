#ifndef ASS2_TAXICENTER_H
#define ASS2_TAXICENTER_H

#include "Trip.h"
#include "Comunicator.h"
#include <pthread.h>
#include "../comunication/Server.h"

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
    Map* map;
    pthread_mutex_t lock;
public:
    TaxiCenter(Protocol* protocol, Tcp* tcp, Map* map);
    ~TaxiCenter();
    void acceptNewDriver();
    void addComunicator(Comunicator* comunicator);
    void addAvaliableTaxi(Taxi *taxi);
    Taxi* searchTaxiById(int id);
    void addTrip(Trip* trip);
    void createRoute();
    void timePassed();
//    DriverInfo * findClosestDriver(Point start);
    Point *getDriverLocation();
    bool shouldStop();
    void addTripToDriver(int time);
    void sendFinish();
};

#endif //ASS2_TAXICENTER_H
