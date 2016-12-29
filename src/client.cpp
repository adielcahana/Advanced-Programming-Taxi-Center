//
// Created by adi on 26/12/16.
//

#include "client.h"

using namespace std;
client::client(Protocol* protocol){
    this->protocol = protocol;
}

client::client(const char* ip_address, const int port_no, Protocol* protocol){
    this->protocol = protocol;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("error creating socket");
    }
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip_address);
    sin.sin_port = htons(port_no);
}

void client::setConnnection(const char* ip_address, const int port_no) {
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("error creating socket");
    }
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip_address);
    sin.sin_port = htons(port_no);
}

client::~client(){
    delete protocol;
    close(sock);
}

void client::send(int numOfMsg) {
    char* msg = (char*) protocol->createMsg(numOfMsg);
    int sent_bytes = sendto(sock, msg, strlen(msg), 0, (struct sockaddr *) &sin, sizeof(sin));
    if (sent_bytes < 0) {
        perror("error writing to socket");
        send(numOfMsg);
    }
}

int client::receive(int scenario){
    struct sockaddr_in from;
    unsigned int from_len = sizeof(struct sockaddr_in);
    memset(&buffer, 0, sizeof(buffer));
    int bytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *) &from, &from_len);
    if (bytes < 0) {
        perror("error reading from socket");
        receive(scenario);
    }
    cout << "got msg: " << buffer << endl;
    return protocol->translate(buffer, scenario);
}
