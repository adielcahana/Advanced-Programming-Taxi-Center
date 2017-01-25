//
// Created by adi on 12/12/16.
//

#ifndef EX2_PARSE_H
#define EX2_PARSE_H

#include "Driver.h"
#include "LuxTaxi.h"
#include <stdexcept>

/******************************************************************************
* Parser: defines a parsing capability for the input.
* the class can verify the input validity and create object accordingly
******************************************************************************/
class Parser {
    string buffer;
public:
    Map* readMap();
    Driver* readDriver();
    Trip* readTrip();
    Taxi* readTaxi();
private:
    bool isValidDriverInput();
    bool isValidTaxiInput();
    bool isValidTripInput();
    vector<Point*>* readObstacles(int width, int length);
};


#endif //EX2_PARSE_H
