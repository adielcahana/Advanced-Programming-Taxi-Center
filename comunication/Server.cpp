#include "Server.h"

Server::~Server() {
    delete this->udp;
    delete this->protocol;
}

void Server::initialize(){
    this->udp->initialize();
}

void Server::send(int numOfMsg){
    string msg = protocol->createMsg(numOfMsg);
    this->udp->sendData(msg);
}

int Server::receive(int scenario){
    this->udp->reciveData(buffer, 1024);
    return this->protocol->translate(buffer,scenario);
}