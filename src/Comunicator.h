#ifndef EX4_COMUNICATOR_H
#define EX4_COMUNICATOR_H

#include "../comunication/Server.h"
#include "../comunication/TaxiCenterProtocol.h"
#include "DriverInfo.h"
#include "AvaliableNotifier.h"
#include <pthread.h>

#define SLEEP 0.01

extern bool can_continue;

class Comunicator: public Server, public AvaliableNotifier {
private:
    DriverInfo* driverInfo;
    pthread_t* thread;
    int nextMission;
    Point* location;
    bool avaliable;
    AvaliableListener* comunicaorListener;
    void setProtocolMap(Map* map);
public:
    Comunicator(Protocol* protocol, Tcp* tcp, Map* map): Server(protocol, tcp){
        setProtocolMap(map);
        this->nextMission = 1;
        this->avaliable = true;
    }
    ~Comunicator();
    pthread_t* getThread();
    void setThread(pthread_t* thread);
    void talkWithDriver();
    DriverInfo* createDriverInfo(string buffer);
    void setTaxi(Taxi* taxi);
    void setTrip(Trip* trip);
    void setNextMission(int mission);
    int getDriverId();
    int getTaxiId();
    Point* getLocation();
    bool isAvaliable();
    static void* wrapTalkWithDriver(void* comunicator);
    void addAvaliableListener(AvaliableListener* al);
    void removeAvaliableListener(AvaliableListener* al);
};

#endif //EX4_COMUNICATOR_H
