#include "Flow.h"
/******************************************************************************
* The Function Operation: flow ctor
******************************************************************************/
Flow::Flow(int port){
    Map* map = parser.readMap();
    this->center = new TaxiCenter(new TaxiCenterProtocol(), new Udp(true, port), map);
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
                this->talkWithDriver();
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
void Flow::run(){
    while(!center->shouldStop()){
        center->timePassed();
    }
}

void Flow::talkWithDriver() {
    int operation = 0;
    DriverInfo* driverInfo = NULL;
    do {
        operation++;
        operation = this->center->receive(operation);
        cout << operation << endl;
        switch (operation){
            case 1:
                driverInfo = this->center->createDriverInfo(this->center->buffer);
                this->center->addDriverInfo(driverInfo);
                this->center->send(1);
                break;
            case 2:
                this->center->setProtocolMap();
                this->center->send(2);
                break;
            case 3:
                this->center->setProtocolTaxi(driverInfo->getTaxiId());
                this->center->send(3);
                break;
            case 4:
                this->center->setProtocolTrip(0);
                this->center->send(4);
                break;
            default:
                this->center->send(0);
        }
    }while(operation < 5);
}

int main(int argc, char* argv[]){
    int port = atoi(argv[1]);
    Flow flow(port);
    while (!flow.shouldStop) {
        flow.initialize();
        if (!flow.shouldStop) flow.run();
    }
}