//
// Created by adi on 15/12/16.
//

#ifndef EX2_NOTRIP_H
#define EX2_NOTRIP_H


#include "../general/AvaliableListener.h"
#include "Comunicator.h"

/******************************************************************************
* CommunicatorListener: implements the AvaliableListener interface,
* and offers a specific capabilty of updating the avalibality of a driver
******************************************************************************/
class CommunicatorListener : public AvaliableListener {
    vector <Comunicator*>* avaliableCommunicators;
    pthread_mutex_t lock;
public:
    CommunicatorListener(vector <Comunicator*>* avaliableCommunicators);
    ~CommunicatorListener();
    void avaliableEvent(AvaliableNotifier *avaliable);
    void unavaliableEvent(AvaliableNotifier *unavaliable);
};


#endif //EX2_NOTRIP_H