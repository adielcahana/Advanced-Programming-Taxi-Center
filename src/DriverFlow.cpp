#include <fstream>
#include "DriverFlow.h"
//_INITIALIZE_EASYLOGGINGPP

/******************************************************************************
* The function Operation: run the driver (client) program
******************************************************************************/
int main(int argc, char** argv) {
    Parser pars;
    stringstream fileName;

    // create driver from input
    Driver *driver = NULL;
    try {
         driver = pars.readDriver();
    }
    catch (runtime_error){
        return 1;
    }
    //fileName << "driver_" << driver->getId() << "_log.txt";
    driver->initialize(argv[1], atoi(argv[2])); //set the Client connection

    //std::ofstream out(fileName.str().c_str());
    //std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    //std::cout.rdbuf(out.rdbuf());

    int operation = 1;
    driver->send(operation); //send driver id
    do {
        operation = driver->receive(1); //confirm msg delivery
        driver->send(operation); //send confirmation or ask again
    } while (operation == 0); //while the msg is not "hello I am sending your map"

    Map* map = NULL;
    while(map == NULL){
        operation = driver->receive(2); //recieve map
        if (operation != 3) { //if msg isnt a map
            driver->send(0); // request data again
            continue;
        }
        try {
            map = Map::deserialize(driver->buffer);
            driver->setMap(map);
        } catch (runtime_error){
            driver->send(0); // request data again
        }
    }

    driver->send(operation); //request a taxi

    Taxi* taxi = NULL;
    while(taxi == NULL){
        operation = driver->receive(operation); //recieve taxi
        if (operation == 0) {
            driver->send(0); //request data again
            continue;
        } else if (operation == 6) { // "time passed"
            driver->timePassed();
            driver->send(6);
            continue;
        }
//        istringstream stream(driver->buffer);
//        streambuf *cin_backup = cin.rdbuf(stream.rdbuf());
//        cin.rdbuf(stream.rdbuf()); //redirect std::cin
        try {
            // create taxi that got from the taxi center
            taxi = pars.readTaxi(driver->buffer);
            driver->setTaxi(taxi);
            driver->send(4);
        } catch (runtime_error){
            driver->send(0); // request data again
        }
//        cin.rdbuf(cin_backup);
    }
    // run until the program end
    while(true) {
        Trip *trip = NULL;
        while (trip == NULL) {
            operation = driver->receive(4); //recieve trip or time passed
            if(operation == 9){ // finish the program
      //          std::cout.rdbuf(coutbuf);
                delete driver;
                return 0;
            }
            if (operation == 0) {
                driver->send(4); //request data again
                continue;
            } else if (operation == 6) { // "time passed"
                driver->timePassed();
                driver->send(6);
                continue;
            } else if (operation == 7) { // "send location"
                driver->send(7);
                continue;
            }
            try {
                // create trip that got from the taxi center
                trip = Trip::fromString(string(driver->buffer));
                driver->newTrip(trip);
            } catch (runtime_error) {
                driver->send(0); // request data again
            }
        }

        driver->send(5);
        // run until the driver finish the trip or the program end
        while (!driver->isAvaliable()) {
            operation = driver->receive(5);
            if (operation == 9){ // finish the program
        //        std::cout.rdbuf(coutbuf);
                delete driver;
                return 0;
            }
            if (operation == 6) { // "time passed"
                driver->timePassed();
                if(driver->isAvaliable()){ // if the driver finish the trip
                    driver->send(8);
//                    driver->receive(6);
                    break;
                }
                driver->send(6);
            }
            else if(operation == 7){ // "send location"
                driver->send(7);
            }
        }
    }
}