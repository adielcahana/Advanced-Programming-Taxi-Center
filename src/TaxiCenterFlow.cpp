#include "TaxiCenterFlow.h"

/******************************************************************************
* The Function Operation: flow ctor
******************************************************************************/
TaxiCenterFlow::TaxiCenterFlow(int port){
    Map* map = NULL;
    while(map == NULL) {
        try {
            map = parser.readMap();
        }
        catch (exception) {
            cout << "-1" << endl;
        }
    }
    this->center = new TaxiCenter(new TaxiCenterProtocol(), new Tcp(true, port), map);
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
* parsing it. all the numbers that are sent are specified in taxiCenterProtocol
******************************************************************************/
void TaxiCenterFlow::initialize(){
    int option;
    char dummy; // variable for '\n'
    Point* p = NULL;
    Trip* trip = NULL;
    Taxi* taxi = NULL;
    bool shouldStop = false; // initialization stop flag
    int id;
    int numOfDrivers;
    while(!shouldStop) {
        cin >> option;
        cin >> noskipws >> dummy; //read '\n'
        switch (option) {
            case 1:
                    // initialize the server
                    center->initialize();
                    cin >> numOfDrivers;
                    cin >> noskipws >> dummy;
                // first talk with the driver
                for(int i = 0; i < numOfDrivers; i++) {
                    this->center->acceptNewDriver();
                }
                break;
            case 2:
                    try {
                        trip = parser.readTrip();
                        center->addTrip(trip);
                    }
                    catch (runtime_error) {
                        cout << "-1" << endl;
                    }
                    catch (out_of_range){
                        cout << "-1" << endl;
                    }
                break;
            case 3:
                    try {
                        taxi = parser.readTaxi();
                        center->addAvaliableTaxi(taxi);
                    }
                    catch (runtime_error) {
                        cout << "-1" << endl;
                    }
                break;
            case 4:
                cin >> id;
                cin >> noskipws >> dummy;
                p = center->getDriverLocation(id);
                if(p != NULL){
                    cout << *p << endl;
                    delete p;
                } else {
                    cout << "-1" << endl;
                }
                break;
            case 7: // update the flow stop flag, and exit the loop
                this->shouldStop = true;
                shouldStop = true;
                this->center->sendFinish(); // send finish the program
                break;
            case 9:
                // set time passed and check if add trip to driver
                curr_time++;
                this->center->addTripToDriver(curr_time);
                this->center->timePassed();
                break;
            default:
                cout << "-1" << endl;
                break;
        }
    }
}

/******************************************************************************
* The Function Operation: run the game step by step
******************************************************************************/
void TaxiCenterFlow::run(){
}

int main(int argc, char* argv[]) {
    // argv[1] = port number
    int port = atoi(argv[1]);
    TaxiCenterFlow flow(port);
    while (!flow.shouldStop) {
        flow.initialize();
        if (!flow.shouldStop) flow.run();
    }
}