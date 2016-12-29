//
// Created by adi on 26/12/16.
//

#ifndef EX4_CLIENT_H
#define EX4_CLIENT_H

#include "Protocol.h"
#include "Udp.h"

class Client {
private:
    Protocol* protocol;
    Udp* udp;
public:
    char buffer[1024];
    Client(Protocol* protocol): protocol(protocol){};
    ~Client();
    void initialize(const char* ip_address, const int port_num);
    void send(int numOfMsg);
    int receive(int scenario);
};


#endif //EX4_CLIENT_H

