
//
// Created by adi on 04/02/17.
//

#ifndef EX4_GUIPROTOCOL_H
#define EX4_GUIPROTOCOL_H

#include "Protocol.h"
#include "../Navigation/Map.h"
using namespace std;
class GuiProtocol: public Protocol {
private:
    string map;
    vector<Point*>* locations;
public:
    GuiProtocol():Protocol(){};
    ~GuiProtocol();
    void setMap(Map* map);
    void setLocations(vector<Point*>* locations);
    virtual string createMsg(int numOfMsg);
    virtual int translate(char *msg, int scenario);
};


#endif //EX4_GUIPROTOCOL_H
