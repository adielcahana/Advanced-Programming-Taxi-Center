//
// Created by adi on 15/12/16.
//

#ifndef EX2_AVALIABLELISTENER_H
#define EX2_AVALIABLELISTENER_H

class AvaliableNotifier;
/******************************************************************************
* AvaliableListener: an observer interface, fot object who wants to be notified
* about other objects Avaliablity for use
******************************************************************************/
class AvaliableListener {
public:
    virtual void avaliableEvent(AvaliableNotifier *avaliable) = 0;
    virtual void unavaliableEvent(AvaliableNotifier *unavaliable) = 0;
};
#endif //EX2_AVALIABLELISTENER_H
