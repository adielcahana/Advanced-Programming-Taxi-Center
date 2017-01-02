#include "DriverInfo.h"

DriverInfo::~DriverInfo() {}

int DriverInfo::getDriverId() {
    return this->driverId;
}

int DriverInfo::getTaxiId() {
    return this->taxiId;
}

DriverInfo::DriverInfo(DriverInfo *&driverInfo) {
    this->taxiId = driverInfo->taxiId;
    this->driverId = driverInfo->driverId;
}
