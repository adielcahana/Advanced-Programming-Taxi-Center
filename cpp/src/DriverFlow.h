//
// Created by adi on 30/12/16.
//

#ifndef EX4_DRIVERFLOW_H
#define EX4_DRIVERFLOW_H

#include "Driver/Driver.h"
#include "general/Parser.h"
#include <sstream>
#include <stdexcept>
//#include "logging/easylogging++.h"

/******************************************************************************
* Flow: handlesthe program flow. this class uses the input parser for driver
* and run the driver program all the numbers that are sent are specified in
* DriverProtocol
******************************************************************************/
class DriverFlow {
private:
    Parser parser;
    Driver* driver;
};

#endif //EX4_DRIVERFLOW_H
