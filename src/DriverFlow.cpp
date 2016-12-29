//
// Created by adi on 30/12/16.
//

#include "DriverFlow.h"

int main(int argc, char** argv) {
    Parser pars;
    Driver *driver = pars.readDriver();
    driver->initialize(argv[1], atoi(argv[2])); //set the Client connection
    int operation = 1;
    driver->send(operation); //send driver id
    do {
        operation = driver->receive(1); //confirm msg delivery
        driver->send(operation); //send confirmation or ask again
    } while (operation == 0);
    Map* map = NULL;
    while(map == NULL){
        driver->receive(0); //recieve taxi
        try {
            map = Map::deserialize(driver->buffer);
            driver->setMap(map);
        } catch (runtime_error){
            driver->send(0); // request data again
        }
    }
    driver->send(3);
    Taxi* taxi = NULL;
    while(taxi == NULL){
        driver->receive(0); //recieve taxi
        istringstream stream(driver->buffer);
        streambuf *cin_backup = cin.rdbuf(stream.rdbuf());
        cin.rdbuf(stream.rdbuf()); //redirect std::cin
        try {
            taxi = pars.readTaxi();
            driver->setTaxi(taxi);
        } catch (runtime_error){
            driver->send(0); // request data again
        }
        cin.rdbuf(cin_backup);
    }
    while(true) {
        Trip *trip = NULL;
        driver->send(4);
        while (trip == NULL) {
            driver->receive(0); //recieve trip
            istringstream stream(driver->buffer);
            streambuf *cin_backup = cin.rdbuf(stream.rdbuf());
            cin.rdbuf(stream.rdbuf()); //redirect std::cin
            try {
                trip = pars.readTrip();
                driver->newTrip(trip);
            } catch (runtime_error) {
                driver->send(0); // request data again
            }
            cin.rdbuf(cin_backup);
        }
        driver->send(5);
        while (!driver->isAvaliable()) {
            operation = driver->receive(4);
            if (operation == 5) {
                driver->timePassed();
            }
            driver->send(operation);
            if (driver->isAvaliable()) {
                driver->send(7);
            }
        }
    }
}