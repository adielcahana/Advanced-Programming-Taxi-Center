//
// Created by adi on 26/12/16.
//

#include "Client.h"

using namespace std;

void client::initialize(){
    this->udp->initialize();
}

client::~client(){
    delete this->udp;
    delete this->protocol;
}

void client::send(int numOfMsg) {
    string msg = protocol->createMsg(numOfMsg);
    this->udp->sendData(msg);
}

int client::receive(int scenario){
    this->udp->reciveData(buffer, 1024);
    return this->protocol->translate(buffer,scenario);
}
