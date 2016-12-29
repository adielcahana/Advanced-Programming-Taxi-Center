#ifndef ASS2_TRIP_H
#define ASS2_TRIP_H

#include "Passenger.h"

/******************************************************************************
* Trip: the route of the driving have a start and end points and the number of
* passengers int the trip, have a price for the trip, know how much meters
 * passed in the trip
******************************************************************************/
class Trip {
private:
    friend class TaxiCenter;
    friend class Driver;
    int id;
    int totalMeterPassed;
    Point start;
    Point end;
    int numOfPass;
    vector <Point*>* route;
    double price;
    int time;
    vector <Passenger*> passengers;
    void addMeter();
public:
    Trip(int id, Point start, Point end, int numOfPass, double price, int time);
    ~Trip();
    int getId();
    Point* getNextPoint();
    double getPrice();
    int getNumPassengers();
    int sumOfSatisfaction();
    string serialize();
};

#endif //ASS2_TRIP_H
