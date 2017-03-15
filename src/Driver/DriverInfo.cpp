#include "DriverInfo.h"

/******************************************************************************
* The function Operation: constructor for driver info
******************************************************************************/
DriverInfo::DriverInfo(DriverInfo *&driverInfo) {
    this->taxiId = driverInfo->taxiId;
    this->driverId = driverInfo->driverId;
}

/******************************************************************************
* The function Operation: return driver id
******************************************************************************/
int DriverInfo::getDriverId() {
    return this->driverId;
}

/******************************************************************************
* The function Operation: return driver's taxi id
******************************************************************************/
int DriverInfo::getTaxiId() {
    return this->taxiId;
}
