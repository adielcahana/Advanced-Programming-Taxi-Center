#ifndef EX4_PROTOCOL_H
#define EX4_PROTOCOL_H

#include <iostream>

using namespace std;

class Protocol{
public:
    virtual string createMsg(int numOfMsg) = 0;
    virtual int translate(char* msg, int scenario) = 0;
};

#endif //EX4_PROTOCOL_H
