#include "DriverInfo.h"

DriverInfo::~DriverInfo() {}

int DriverInfo::getDriverId() {
    return this->driverId;
}

int DriverInfo::getTaxiId() {
    return this->taxiId;
}
