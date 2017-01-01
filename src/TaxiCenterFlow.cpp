#include "TaxiCenterFlow.h"
/******************************************************************************
* The Function Operation: flow ctor
******************************************************************************/
TaxiCenterFlow::TaxiCenterFlow(int port){
    Map* map = parser.readMap();
    this->center = new TaxiCenter(new TaxiCenterProtocol(), new Udp(true, port), map);
    this->shouldStop = false;
}
/******************************************************************************
* The Function Operation: flow dtor
******************************************************************************/
TaxiCenterFlow::~TaxiCenterFlow(){
    delete center;
}
/******************************************************************************
* The Function Operation: initialize the game while getting input and
* parsing it
******************************************************************************/
void TaxiCenterFlow::initialize(){
    int option;
    int time = 0;
    char dummy; // variable for '\n'
    Point* p = NULL;
    bool shouldStop = false; // initialization stop flag
    bool wasInitialize = false;
    int id;
    while(!shouldStop) {
        cin >> option;
        cin >> noskipws >> dummy; //read '\n'
        switch (option) {
            case 1:
                if (!wasInitialize) {
                    center->initialize();
                    cout << "bind" << endl;
                }
                this->center->talkWithDriver();
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
                } else {
                    cout << "driver id doesn't exist" << endl;
                }
                break;
            case 6: // stop getting input, and exit the loop
                shouldStop = true;
                break;
            case 7: // update the flow stop flag, and exit the loop
                this->shouldStop = true;
                shouldStop = true;
                break;
            case 9:
                time++;
                this->center->timePassed();
            default:
                break;
        }
    }
}

/******************************************************************************
* The Function Operation: run the gmae step by step
******************************************************************************/
void TaxiCenterFlow::run(){
    while(!center->shouldStop()){
        center->timePassed();
    }
}

int main(int argc, char* argv[]){
    // argv[1] = port number
    int port = atoi(argv[1]);
    TaxiCenterFlow flow(port);
    while (!flow.shouldStop) {
        flow.initialize();
        if (!flow.shouldStop) flow.run();
    }
}