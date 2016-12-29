//
// Created by adi on 26/12/16.
//

#ifndef EX4_CLIENT_H
#define EX4_CLIENT_H

#include "Protocol.h"
#include "Udp.h"

class client {
private:
    Protocol* protocol;
    Udp* udp;
public:
    char buffer[1024];
    client(Protocol* protocol, Udp* udp): udp(udp), protocol(protocol){};
    ~client();
    void initialize();
    void send(int numOfMsg);
    int receive(int scenario);
};


#endif //EX4_CLIENT_H

