#ifndef EX4_COMUNICATOR_H
#define EX4_COMUNICATOR_H

#include <pthread.h>
#include "../comunication/Server.h"
#include "../comunication/TaxiCenterProtocol.h"
#include "../Driver/DriverInfo.h"
#include "../general/AvaliableNotifier.h"

#define SLEEP 0.01

// global boolean variable
extern bool can_continue;
extern int curr_time;

/******************************************************************************
* Comunicator: created by the taxi center to comunicate with the driver in a
* different thread contain the driver info and the socket that connect to the
* client
******************************************************************************/
class Comunicator: public Server, public AvaliableNotifier {
private:
    DriverInfo* driverInfo;
    pthread_t* thread;
    Point* location;
    int nextMission;
    int time;
    bool avaliable;
    AvaliableListener* comunicaorListener;
    void setProtocolMap(Map* map);
public:
    Comunicator(Protocol* protocol, Tcp* tcp, Map* map): Server(protocol, tcp){
        setProtocolMap(map);
        time = 0;
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
    int getTime();
    Point* getLocation();
    bool isAvaliable();
    static void* wrapTalkWithDriver(void* comunicator);
    void addAvaliableListener(AvaliableListener* al);
    void removeAvaliableListener();
};

#endif //EX4_COMUNICATOR_H
