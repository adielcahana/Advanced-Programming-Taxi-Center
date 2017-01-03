#ifndef ASS2_TAXI_H
#define ASS2_TAXI_H

#include "Map.h"
#include "Trip.h"
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//#include <boost/tokenizer.hpp>
//#include <boost/algorithm/string/predicate.hpp>
//#include <boost/lexical_cast.hpp>
//#include <boost/assign/list_of.hpp>
//#include <boost/algorithm/string.hpp>
//#include <boost/iostreams/device/back_inserter.hpp>
//#include <boost/iostreams/stream.hpp>
//#include <boost/archive/binary_oarchive.hpp>
//#include <boost/archive/binary_iarchive.hpp>

//using namespace boost::archive;

/******************************************************************************
* Status: enum for Manufacturer
******************************************************************************/
enum Manufacturer {HONDA, SUBARO, TESLA, FIAT};

/******************************************************************************
* Status: enum for color
******************************************************************************/
enum Color {RED, BLUE, GREEN, PINK, WHITE};

/******************************************************************************
* Taxi: regular taxi, the driver's cab. have id and total Km passed, tariff
* for a ride the manufacturer and a color. know his location. move one block
* in a time
******************************************************************************/
class Taxi{
//    friend class boost::serialization::access;
protected:
    int id;
    float totalKm;
    Manufacturer manufacturer;
    Color color;
    float tariff;
    Point* location;
    void addKm(float km);
//    template<class Archive>
//    void serialize(Archive &ar, const unsigned int version)
//    {
//        ar & id;
//        ar & manufacturer;
//        ar & color;
//    }
public:
    Taxi(int id, Manufacturer type, Color color, float tariff = 1):
            id(id), manufacturer(type), color(color),totalKm(0), tariff(1) {location = new Point(0,0);};
    Taxi(int id, Manufacturer type, Color color, Point* location):
            id(id), manufacturer(type), color(color),totalKm(0), tariff(1),
            location(location){};
    Taxi(Taxi &other);
//    Taxi(){};
    ~Taxi();
    int getId();
    float getKm();
    float getTariff();
    virtual int getVelocity();
    virtual void moveOneStep(Point* point);
    void updateLocation(Point* location);
    Point* getLocation();
    bool operator==(const Taxi &rhs) const;
    bool operator!=(const Taxi &rhs) const;
    string toString();
};

#endif
