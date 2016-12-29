#ifndef ASS2_DRIVER_H
#define ASS2_DRIVER_H

#include "Taxi.h"
#include "Trip.h"
#include "AvaliableNotifier.h"
#include "../comunication/Client.h"
#include "../comunication/DriverProtocol.h"

/******************************************************************************
* Status: enum for the driver mariatal status
******************************************************************************/
enum Status {SINGLE, MARRIED, DIVORCED, WIDOWED};

/******************************************************************************
* Driver: represent the driver of the taxi have id, age, marital status, years
* of exprience. move on the map with taxi and get point from his trip.
* have a satisfacation from the passengrs and know his location on the map
******************************************************************************/
class Driver: public AvaliableNotifier, public Client{
private:
    int id;
    int age;
    Status maritalStat;
    int exp;
    Taxi* taxi;
    int taxiId;
    Trip* trip;
    Map* map;
    float avgSatisfaction;
    int passCount;
    Point* location;
    void calcAvg();
    void addPassCount(int passengers);
public:
    Driver(int id, int age, Status status, int exp, int taxiId):
            id(id), age(age),maritalStat(status), exp(exp), taxiId(taxiId),
            trip(NULL), location(new Point(0,0)),Client((Protocol*) new DriverProtocol(id, &location)){};
    ~Driver();
    int getId();
    void setMap(Map* map);
    Point* getLocation();
    void setTaxi(Taxi* taxi);
    int getTaxiId();
    Taxi* getTaxi();
    void newTrip(Trip* trip);
    void timePassed();
    void moveOneStep();
    void addAvaliableListener(AvaliableListener* hl);
    void removeAvaliableListener(AvaliableListener* hl);
    void notifyAvaliable();
    void notifyUnavaliable();
    bool isAvaliable();
};

#endif
