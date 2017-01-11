//
// Created by adi on 26/12/16.
//

#include "Client.h"

using namespace std;

/******************************************************************************
* The function Operation: Client destructor - delete the udp and the protocol.
******************************************************************************/
Client::~Client(){
    delete this->tcp;
    delete this->protocol;
}

/******************************************************************************
* The function Operation: initialize the Client create new Udp and initilize it
******************************************************************************/
void Client::initialize(const char* ip_address, const int port_num){
    this->tcp = new Tcp(false, port_num, ip_address);
    this->tcp->initialize();
}

/******************************************************************************
* The function Operation: get a number of message (for the protocol) create a
* message and send it to server
******************************************************************************/
void Client::send(int numOfMsg) {
    string msg = protocol->createMsg(numOfMsg);
    this->tcp->sendData(msg);
}

/******************************************************************************
* The function Operation: get a scenario (for the protocol) get message from
* the server and send to protocol for translate
******************************************************************************/
int Client::receive(int scenario){
    this->tcp->reciveData(buffer, sizeof(buffer));
    return this->protocol->translate(buffer,scenario);
}