//
// Created by adi on 26/12/16.
//

#ifndef EX4_PROTOCOL_H
#define EX4_PROTOCOL_H
class Protocol{
public:
    virtual int translate(char* msg, int scenario) = 0;
    virtual const char* createMsg(int numOfMsg) = 0;
};
#endif //EX4_PROTOCOL_H
