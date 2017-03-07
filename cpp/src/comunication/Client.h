//
// Created by adi on 26/12/16.
//

#ifndef EX4_CLIENT_H
#define EX4_CLIENT_H

#include "Protocol.h"
#include "Tcp.h"

/******************************************************************************
* Client: the class for the client has a protocol and udp. client class manage
* the comunication with the server for send and recive messages
******************************************************************************/
class Client {
private:
    Protocol* protocol;
    Tcp* tcp;
public:
    char buffer[65536];
    Client(Protocol* protocol): protocol(protocol){};
    ~Client();
    void initialize(const char* ip_address, const int port_num);
    void send(int numOfMsg);
    int receive(int scenario);
};


#endif //EX4_CLIENT_H

