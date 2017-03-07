//
// Created by ori on 29/12/16.
//

#ifndef EX4_DRIVERINFO_H
#define EX4_DRIVERINFO_H

/******************************************************************************
* DriverInfo: class that contain info of the driver (driver id and taxi id)
******************************************************************************/
class DriverInfo {
private:
    int driverId;
    int taxiId;
public:
    DriverInfo(int driverId = 0, int taxiId = 0): driverId(driverId), taxiId(taxiId){};
    DriverInfo(DriverInfo *&pInfo);
    int getDriverId();
    int getTaxiId();
};


#endif //EX4_DRIVERINFO_H
