//
// Created by adi on 15/12/16.
//

#include "TripListener.h"
/******************************************************************************
* The function Operation: adds the notifyer driver to the driver list
******************************************************************************/
void TripListener::avaliableEvent(AvaliableNotifier *avaliable) {
    this->avaliableDrivers->push_back((Driver*) avaliable);
}
/******************************************************************************
* The function Operation: removes the notifyer driver from the driver list
******************************************************************************/
void TripListener::unavaliableEvent(AvaliableNotifier *unavaliable){
    for(int i = 0; i < this->avaliableDrivers->size(); i++) {
        if (this->avaliableDrivers->at(i) == unavaliable){
            this->avaliableDrivers->at(i) = NULL;
            this->avaliableDrivers->erase(avaliableDrivers->begin() + i);
            return;
        }
    }
}