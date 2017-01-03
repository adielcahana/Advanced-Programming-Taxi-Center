//
// Created by adi on 30/12/16.
//

#ifndef EX4_DRIVERFLOW_H
#define EX4_DRIVERFLOW_H
#include "src/Driver.h"
#include "src/Parser.h"
#include <sstream>

/******************************************************************************
* Flow: handlesthe program flow. this class uses the input parser for driver
* and run the driver program
******************************************************************************/
class DriverFlow {
private:
    Parser parser;
    Driver* driver;
};


#endif //EX4_DRIVERFLOW_H
