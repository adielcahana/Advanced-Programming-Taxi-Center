//
// Created by adi on 26/12/16.
//

#ifndef EX4_CLIENT_H
#define EX4_CLIENT_H

#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "Protocol.h"

class client {
private:
    Protocol* protocol;
    int sock;
    struct sockaddr_in sin;
public:
    char buffer[4096];
    client(Protocol* protocol);
    client(const char* ip_address, const int port_no, Protocol* protocol);
    ~client();
    void setConnnection(const char* ip_address, const int port_no);
    void send(int numOfMsg);
    int receive(int scenario);
};


#endif //EX4_CLIENT_H

