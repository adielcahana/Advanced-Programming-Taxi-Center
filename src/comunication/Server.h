#ifndef EX4_SERVER_H
#define EX4_SERVER_H

#include <iostream>
#include "Tcp.h"
#include "Protocol.h"


/******************************************************************************
* Server: the class for the server has a protocol and udp. server class manage
* the comunication with clients for send and recive messages
******************************************************************************/
class Server {
protected:
    Protocol* protocol;
    Tcp* tcp;
public:
    char buffer[65536];
    Server(Protocol* protocol, Tcp* tcp): tcp(tcp), protocol(protocol){};
    ~Server();
    void initialize();
    Tcp* accept();
    void send(int numOfMsg);
    int receive(int scenario);
};


#endif //EX4_SERVER_H
