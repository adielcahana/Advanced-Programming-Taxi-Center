#include "TaxiCenterFlow.h"

/******************************************************************************
* The Function Operation: flow ctor
******************************************************************************/
TaxiCenterFlow::TaxiCenterFlow(Tcp* tcp) : Server(new GuiProtocol(), tcp){
    Map* map = NULL;
    while(map == NULL) {
        try {
            map = parser.readMap();
        }
        catch (exception) {
            cout << "-1" << endl;
        }
    }
    this->center = new TaxiCenter(new TaxiCenterProtocol(), tcp, map);
    this->shouldStop = false;
    // initialize the server
    initialize();
    this->tcp = this->accept();
    //send the map to the gui
    ((GuiProtocol*) this->protocol)->setMap(map);
    this->send(1);
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
void TaxiCenterFlow::run(){
    int option;
//    char dummy; // variable for '\n'
    Point* p = NULL;
    Trip* trip = NULL;
    Taxi* taxi = NULL;
    bool shouldStop = false; // initialization stop flag
    int id;
    int numOfDrivers;
    stringstream str;
    while(!shouldStop) {
        option = receive(0);
//        cin >> option;
//        cin >> noskipws >> dummy; //read '\n'
        switch (option) {
            case 1:
                str.str(buffer);
                // get the driver id
                str >> numOfDrivers;
                // first talk with the driver
                for(int i = 0; i < numOfDrivers; i++) {
                    this->center->acceptNewDriver();
                }
                break;
            case 2:
                try {
                    trip = parser.readTrip(buffer);
                    send(0);
                    center->addTrip(trip);
                }
                catch (runtime_error) {
                    send(-1);
//                    cout << "-1" << endl;
                }
                catch (out_of_range){
                    send(-1);
//                    cout << "-1" << endl;
                }
                break;
            case 3:
                    try {
                        taxi = parser.readTaxi(buffer);
                        send(0);
                        center->addAvaliableTaxi(taxi);
                    }
                    catch (runtime_error) {
                        send(-1);
//                        cout << "-1" << endl;
                    }
                break;
//            case 4:
//                cin >> id;
//                cin >> noskipws >> dummy;
//                p = center->getDriverLocation(id);
//                if(p != NULL){
//                    cout << *p << endl;
//                    delete p;
//                } else {
//                    cout << "-1" << endl;
//                }
//                break;
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
                ((GuiProtocol*) this->protocol)->setLocations(this->center->getAllDriversLocations());
                send(2);
                break;
            default:
                continue;
//                cout << "-1" << endl;
//                break;
        }
    }
}


int main(int argc, char* argv[]) {
    // argv[1] = port number
    int port = atoi(argv[1]);
    Tcp* tcp = new Tcp(true , port);
    TaxiCenterFlow flow(tcp);
    while (!flow.shouldStop) {
        flow.run();
        if (!flow.shouldStop) flow.run();
    }
}