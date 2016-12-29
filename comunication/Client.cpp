//
// Created by adi on 26/12/16.
//

#include "Client.h"

using namespace std;

void Client::initialize(const char* ip_address, const int port_num){
    this->udp = new Udp(false, port_num, ip_address);
    this->udp->initialize();
}

Client::~Client(){
    delete this->udp;
    delete this->protocol;
}

void Client::send(int numOfMsg) {
    string msg = protocol->createMsg(numOfMsg);
    this->udp->sendData(msg);
}

int Client::receive(int scenario){
    this->udp->reciveData(buffer, 1024);
    return this->protocol->translate(buffer,scenario);
}