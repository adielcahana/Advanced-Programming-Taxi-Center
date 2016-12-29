#include "Parser.h"
/******************************************************************************
* The Function Operation: verify valid trip input
******************************************************************************/
bool Parser::isValidTripInput(){
    // flag for reading comma
    bool readComma = true;
    // flag for reading dot. only one time allowed for the tariff
    bool readDot = false;
    int commaCounter = 0;
    for(int i=0; i<buffer.size() && commaCounter <= 6; i++){
        if(readComma){ // after a comma, there should be only digits
            if(!isdigit(buffer[i])){
                return false;
            }
            readComma = false;
        } else { // after a digit, there can be a digit, or a comma, or dot
            if(buffer[i] == ',') {
                readComma = true;
                commaCounter++;
                continue;
            // a dot can be only after the 6th comma, at tariff
            }else if(commaCounter == 6 && buffer[i] == '.' && !readDot) {
                readDot = true;
                continue;
            } else if(!isdigit(buffer[i])){
                return false;
            }
        }
    }
    // if all the other tests passed, verify that there are only 6 comma
    return commaCounter == 6;
}
/******************************************************************************
* The Function Operation: verify valid Taxi input
******************************************************************************/
bool Parser::isValidTaxiInput(){
    int commaCounter = 0;
    for(int i=0; i<buffer.size() && commaCounter <= 3; i++){
        if(commaCounter < 2){ // digits should be before the 2nd comma
            if(buffer[i] == ','){
                commaCounter++;
                continue;
            } else if(!isdigit(buffer[i])){
                return false;
            }
        } else { // Letters should be after the 2nd comma
            if(buffer[i] == ','){
                commaCounter++;
                continue;
            } else if(!isalpha(buffer[i])){
                return false;
            }
        }
    }
    // if all the other tests passed, verify that there are only 3 comma
    return commaCounter == 3;
}
/******************************************************************************
* The Function Operation: verify valid Driver input
******************************************************************************/
bool Parser::isValidDriverInput() {
    int commaCounter = 0;
    // flag for reading letter
    bool readAlpha = false;
    for(int i=0; i<buffer.size() && commaCounter <= 4; i++){
        if(commaCounter == 2 && !readAlpha){ // letter should appear only once, after the 2nd comma
            if(!isalpha(buffer[i])) {
                return false;
            }
            readAlpha = true;
            continue;
        } else { // otherwise, digits or comma should appear
            if(buffer[i] == ','){
                commaCounter++;
                continue;
            } else if(!isdigit(buffer[i])){
                return false;
            }
        }
    }
    // if all the other tests passed, verify that there are only 4 comma
    return commaCounter == 4;
}
/******************************************************************************
* The Function Operation: read map input
******************************************************************************/
Map* Parser::readMap() {
    getline(cin, buffer);
    //create c-string from buffer
    char *c = new char[buffer.length() + 1];
    strcpy(c, buffer.c_str());
    int width, length;
    char *x, *y;
    vector<Point *> *obstacles;
    try {
        //verify that x and y are digits
        if ((x = strtok(c, " ")) == NULL) throw runtime_error("bad arguments for map");
        for (int i = 0; x[i] != 0; i++) {
            if (!isdigit(x[i])) throw runtime_error("bad arguments for map");
        }
        if ((y = strtok(NULL, " ")) == NULL) throw runtime_error("bad arguments for map");
        for (int i = 0; y[i] != 0; i++) {
            if (!isdigit(y[i])) throw runtime_error("bad arguments for map");
        }
        //convert x,y to int
        width = atoi(x);
        length = atoi(y);
        obstacles = this->readObstacles();
    }
    catch (exception){
        delete[](c);
        throw runtime_error("bad arguments for map");
    }
    delete[](c);
    return new Map(width, length, obstacles);
}
/******************************************************************************
* The Function Operation: read driver input
******************************************************************************/
Driver* Parser::readDriver(){
    getline(cin, buffer);
    if(!isValidDriverInput()) throw runtime_error("bad argument for new driver");
    //create c-string from buffer
    char *c = new char[buffer.length() + 1];
    strcpy(c, buffer.c_str());
    int id, age, exp, vehicle_id;
    id = atoi(strtok(c, ","));
    age = atoi(strtok(NULL, ","));
    Status stat;
    switch(strtok(NULL, ",")[0]){
        case 'S':
            stat = SINGLE;
            break;
        case 'M':
            stat = MARRIED;
            break;
        case 'D':
            stat = DIVORCED;
            break;
        case 'W':
            stat = WIDOWED;
            break;
        default:
            delete[](c);
            throw runtime_error("bad argument for new driver");
    }
    exp = atoi(strtok(NULL, ","));
    vehicle_id = atoi(strtok(NULL, ","));
    delete[](c);
    return new Driver(id,age,stat,exp,vehicle_id);
}
/******************************************************************************
* The Function Operation: read trip input
******************************************************************************/
Trip* Parser::readTrip(){
    getline(cin, buffer);
    if (!isValidTripInput()) throw runtime_error("bad argument for new Trip");
    //create c-string from buffer
    char *c = new char[buffer.length() + 1];
    strcpy(c, buffer.c_str());
    int id = atoi(strtok(c, ","));
    char *x = strtok(NULL, ",");
    char *y = strtok(NULL, ",");
    Point start(atoi(x),atoi(y));
    x = strtok(NULL, ",");
    y = strtok(NULL, ",");
    Point end(atoi(x),atoi(y));
    int numOfPassengers = atoi(strtok(NULL, ","));
    double tariff = strtod(strtok(NULL, ","), NULL);
    delete[] (c);
    return new Trip(id, start, end, numOfPassengers, tariff);
}
/******************************************************************************
* The Function Operation: read taxi input
******************************************************************************/
Taxi* Parser::readTaxi(){
    getline(cin, buffer);
    if (!isValidTaxiInput()) throw runtime_error("bad argument for new taxi");
    //create c-string from buffer
    char *c = new char[buffer.length() + 1];
    strcpy(c, buffer.c_str());
    int id, taxiType;
    Manufacturer manufacturer;
    Color color;
    try {
        id = atoi(strtok(c, ","));
        taxiType = atoi(strtok(NULL, ","));
        //switch case for correct manufacturer input
        switch (strtok(NULL, ",")[0]) {
            case 'H':
                manufacturer = HONDA;
                break;
            case 'S':
                manufacturer = SUBARO;
                break;
            case 'T':
                manufacturer = TESLA;
                break;
            case 'F':
                manufacturer = FIAT;
                break;
            default:
                throw runtime_error("bad argument for taxi manufacturer");
        }
        //switch case for correct color input
        switch (strtok(NULL, ",")[0]) {
            case 'R':
                color = RED;
                break;
            case 'B':
                color = BLUE;
                break;
            case 'G':
                color = GREEN;
                break;
            case 'P':
                color = PINK;
                break;
            case 'W':
                color = WHITE;
                break;
            default:
                throw runtime_error("bad argument for taxi color");
        }
    }
    catch (exception){
        delete[](c);
        throw runtime_error("bad argument for taxi color");
    }
    delete[](c);
    //create correct taxi type
    if (taxiType == 1) {
        return new Taxi(id, manufacturer, color);
    } else {
        return new LuxTaxi(id, manufacturer, color);
    }
}
/******************************************************************************
* The Function Operation: read Obstacles input, pont by point
******************************************************************************/
vector<Point*>* Parser::readObstacles(){
    vector<Point*>* obstacles = new vector<Point*>();
    //get number of obstacles
    getline(cin, buffer);
    //create c-string from buffer
    char *c = new char[buffer.length() + 1];
    strcpy(c, buffer.c_str());
    //verify that c is digits
    for (int i = 0; c[i] != 0; i++) {
        if (!isdigit(c[i])) {
            throw runtime_error("bad arguments for obstacles");
        }
    }
    int numOfObsatcles = atoi(c);
    delete[](c);
    for(int i = 0; i < numOfObsatcles; i++) {
        try {
            //get point line
            getline(cin, buffer);
            obstacles->push_back(Point::deserialize(buffer));
        }
        catch (exception) {
            // deallocate all points
            for (int i = 0; i < obstacles->size(); i++) {
                delete obstacles->at(i);
            }
            delete obstacles;
            throw runtime_error("bad arguments for obstacles");
        }
    }
    return obstacles;
}