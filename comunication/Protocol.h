#ifndef EX4_PROTOCOL_H
#define EX4_PROTOCOL_H

#include <iostream>

using namespace std;

/******************************************************************************
* Protocol: interface for protocol has pure virtual function for create
* messages and translate them
******************************************************************************/
class Protocol{
public:
/******************************************************************************
* The Function Operation: get number of message and create the right message
******************************************************************************/
    virtual string createMsg(int numOfMsg) = 0;

/******************************************************************************
* The Function Operation: get message and excpected scenario and return
* the right scenario for the massege
******************************************************************************/
    virtual int translate(char* msg, int scenario) = 0;
};

#endif //EX4_PROTOCOL_H
