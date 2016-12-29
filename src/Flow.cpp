#include "Flow.h"
/******************************************************************************
* The Function Operation: flow ctor
******************************************************************************/
Flow::Flow(){
    Map* map = parser.readMap();
    this->center = new TaxiCenter(map);
    this->shouldStop = false;
}
/******************************************************************************
* The Function Operation: flow dtor
******************************************************************************/
Flow::~Flow(){
    delete center;
}
/******************************************************************************
* The Function Operation: initialize the game while getting input and
* parsing it
******************************************************************************/
void Flow::initialize(){
    int option;
    char dummy; // variable for '\n'
    Point* p = NULL;
    bool shouldStop = false; // initialization stop flag
    int id;
    while(!shouldStop){
        cin >> option;
        cin >> noskipws >> dummy; //read '\n'
        switch(option){
            case 1:
                center->addDriver(parser.readDriver());
                break;
            case 2:
                center->addTrip(parser.readTrip());
                break;
            case 3:
                center->addAvaliableTaxi(parser.readTaxi());
                break;
            case 4:
                cin >> id;
                cin >> noskipws >> dummy;
                p = center->getDriverLocation(id);
                if(p != NULL){
                    cout << *p << endl;
                    delete p;
                }
                else {
                    cout << "driver id doesn't exist" << endl;
                }
                break;
            case 6: // stop getting input, and exit the loop
                shouldStop = true;
                break;
            case 7: // update the flow stop flag, and exit the loop
                this->shouldStop = true;
                shouldStop = true;
            default:
                break;
        }
    }
}
/******************************************************************************
* The Function Operation: run the gmae step by step
******************************************************************************/
void Flow::run(){
    while(!center->shouldStop()){
        center->timePassed();
    }
}
