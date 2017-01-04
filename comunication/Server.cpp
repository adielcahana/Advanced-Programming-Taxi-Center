#include "Server.h"

/******************************************************************************
* The function Operation: Server destructor - delete the udp.
******************************************************************************/
Server::~Server() {
    delete this->udp;
}

/******************************************************************************
* The function Operation: initialize the server
******************************************************************************/
void Server::initialize(){
    // initilize the udp (to do bind)
    this->udp->initialize();
}

/******************************************************************************
* The function Operation: get a number of message (for the protocol) create a
* message and send it to client
******************************************************************************/
void Server::send(int numOfMsg){
    string msg = protocol->createMsg(numOfMsg);
    this->udp->sendData(msg);
}

/******************************************************************************
* The function Operation: get a scenario (for the protocol) get message from
* the client and send to protocol for translate
******************************************************************************/
int Server::receive(int scenario){
    this->udp->reciveData(buffer, 1024);
    return this->protocol->translate(buffer,scenario);
}