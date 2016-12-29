#include "Passenger.h"

/******************************************************************************
* The function Operation: give a random satisfacation between 1 to 5
******************************************************************************/
int Passenger::satisfacation(){
    srand (time(NULL));
    return rand() % 5 + 1;
}