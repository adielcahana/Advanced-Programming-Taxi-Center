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
* parsing it. all the numbers that are sent are specified in taxiCenterProtocol
******************************************************************************/
void TaxiCenterFlow::initialize(){
    int option, status = 0, time = 0;
    char dummy; // variable for '\n'
    Point* p = NULL;
    Trip* trip = NULL;
    bool shouldStop = false; // initialization stop flag
    bool wasInitialize = false;
    int id;
    while(!shouldStop) {
        cin >> option;
        cin >> noskipws >> dummy; //read '\n'
        switch (option) {
            case 1:
                if (!wasInitialize) {
                    // initialize the server
                    center->initialize();
                    cin >> option;
                    cin >> noskipws >> dummy;
                }
                // first talk with the driver
                for(int i = 0; i < option; i++) {
                    this->center->talkWithDriver();
                }
                break;
            case 2:
                trip = parser.readTrip();
                center->addTrip(trip);
                trip->setThread(new pthread_t());
                status = pthread_create(trip->getThread(), NULL, TaxiCenterFlow::createRoute, this->center);
                if (status) {
                    cout << "error while trying to create trip" << endl;
                }
                break;
            case 3:
                center->addAvaliableTaxi(parser.readTaxi());
                break;
            case 4:
                cin >> id;
                cin >> noskipws >> dummy;
                p = center->getDriverLocation();
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
                this->center->send(8); // send finish the program
                break;
            case 9:
                // set time passed and check if add trip to driver
                time++;
                this->center->addTripToDriver(time);
                this->center->timePassed();
            default:
                break;
        }
    }
}

/******************************************************************************
* The Function Operation: run the game step by step
******************************************************************************/
void TaxiCenterFlow::run(){
    while(!center->shouldStop()){
        center->timePassed();
    }
}

void* TaxiCenterFlow::createRoute(void* center){
    ((TaxiCenter*) center)->createRoute();
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    std::ifstream in("server.txt");
    std::streambuf *cinbuf = std::cin.rdbuf(); //save old buf
    std::cin.rdbuf(in.rdbuf());

    std::ofstream out("taxi_center_log.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf());
    // argv[1] = port number
    int port = atoi(argv[1]);
    TaxiCenterFlow flow(port);
    while (!flow.shouldStop) {
        flow.initialize();
        if (!flow.shouldStop) flow.run();
    }
    std::cout.rdbuf(coutbuf);
    std::cin.rdbuf(cinbuf);
}