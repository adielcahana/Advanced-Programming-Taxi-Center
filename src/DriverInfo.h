//
// Created by ori on 29/12/16.
//

#ifndef EX4_DRIVERINFO_H
#define EX4_DRIVERINFO_H


class DriverInfo {
private:
    int driverId;
    int taxiId;
public:
    DriverInfo(int driverId, int taxiId): driverId(driverId), taxiId(taxiId){};
    ~DriverInfo();
    int getDriverId();
    int getTaxiId();
};


#endif //EX4_DRIVERINFO_H
