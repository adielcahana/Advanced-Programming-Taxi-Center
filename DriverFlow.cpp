#include "DriverFlow.h"
#include <stdexcept>

#include "boost/serialization/export.hpp"

//BOOST_CLASS_EXPORT_GUID(LuxTaxi,"LuxTaxi")

//string bufferToString(char* buffer, int len){
//    string ret(buffer,len);
//    return ret;
//}
/******************************************************************************
* The function Operation: run the driver (client) program
******************************************************************************/
int main(int argc, char** argv) {
    Parser pars;
    // create driver from input
    Driver *driver = pars.readDriver();
    driver->initialize(argv[1], atoi(argv[2])); //set the Client connection
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
        istringstream stream(driver->buffer);
        streambuf *cin_backup = cin.rdbuf(stream.rdbuf());
        cin.rdbuf(stream.rdbuf()); //redirect std::cin
        try {
            // create taxi that got from the taxi center
            taxi = pars.readTaxi();
//            char buffer[1024];
//            string serial_str = bufferToString(buffer, sizeof(buffer));
//            boost::iostreams::basic_array_source<char> device(serial_str.c_str(), serial_str.size());
//            boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s2(device);
//            boost::archive::binary_iarchive ia(s2);
//            ia >> *taxi;
            driver->setTaxi(taxi);
            driver->send(4);
        } catch (runtime_error){
            driver->send(0); // request data again
        }
        cin.rdbuf(cin_backup);
    }
    // run until the program end
    while(true) {
        Trip *trip = NULL;
        while (trip == NULL) {
            operation = driver->receive(4); //recieve trip or time passed
            if(operation == 9){ // finish the program
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
            istringstream stream(driver->buffer);
            streambuf *cin_backup = cin.rdbuf(stream.rdbuf());
            cin.rdbuf(stream.rdbuf()); //redirect std::cin
            try {
                // create trip that got from the taxi center
                trip = pars.readTrip();
                driver->newTrip(trip);
            } catch (runtime_error) {
                driver->send(0); // request data again
            }
            cin.rdbuf(cin_backup);
        }

        driver->send(5);
        // run until the driver finish the trip or the program end
        while (!driver->isAvaliable()) {
            operation = driver->receive(5);
            if(operation == 9){ // finish the program
                delete driver;
                return 0;
            }
            if (operation == 6) { // "time passed"
                driver->timePassed();
                if(driver->isAvaliable()){ // if the driver finish the trip
                    driver->send(8);
                    driver->receive(6);
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