//
// Created by adi on 15/12/16.
//

#include "CommunicatorListener.h"

CommunicatorListener::CommunicatorListener(vector <Comunicator*>* avaliableCommunicators) {
    this->avaliableCommunicators =avaliableCommunicators;
    if (pthread_mutex_init(&lock, NULL) != 0) {
        cout << "unable to init CommunicatorListener lock" << endl;
    }
}
CommunicatorListener::~CommunicatorListener(){
    pthread_mutex_destroy(&lock);
}

/******************************************************************************
* The function Operation: adds the notifyer driver to the driver list
******************************************************************************/
void CommunicatorListener::avaliableEvent(AvaliableNotifier* avaliable) {
    pthread_mutex_lock(&lock);
    this->avaliableCommunicators->push_back((Comunicator*) avaliable);
    pthread_mutex_unlock(&lock);
}
/******************************************************************************
* The function Operation: removes the notifyer driver from the driver list
******************************************************************************/
void CommunicatorListener::unavaliableEvent(AvaliableNotifier *unavaliable){
    pthread_mutex_lock(&lock);
    for(int i = 0; i < this->avaliableCommunicators->size(); i++) {
        if (this->avaliableCommunicators->at(i) == (Comunicator*) unavaliable){
            this->avaliableCommunicators->at(i) = NULL;
            this->avaliableCommunicators->erase(avaliableCommunicators->begin() + i);
            pthread_mutex_unlock(&lock);
            break;
        }
    }
}