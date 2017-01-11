#ifndef EX4_COMUNICATOR_H
#define EX4_COMUNICATOR_H

#include "../comunication/Server.h"
#include "../comunication/TaxiCenterProtocol.h"
#include "DriverInfo.h"
#include <pthread.h>

class Comunicator: public Server {
private:
    DriverInfo* driverInfo;
    pthread_t* thread;
public:
    Comunicator(Protocol* protocol, Tcp* tcp): Server(protocol, tcp){};
    ~Comunicator();
    void talkWithDriver(Map* map, Taxi* taxi);
    DriverInfo* createDriverInfo(string buffer);
    void setProtocolMap(Map* map);
    void setProtocolTaxi(Taxi* taxi);
    void setProtocolTrip(Trip* trip);
};


#endif //EX4_COMUNICATOR_H
