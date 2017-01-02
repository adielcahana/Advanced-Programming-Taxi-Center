#include <sstream>
#include "Taxi.h"

/******************************************************************************
* The function Operation: Taxi constructor.
******************************************************************************/
Taxi::Taxi(Taxi& other){
    id = other.id;
    totalKm = other.totalKm;
    manufacturer = other.manufacturer;
    color = other.color;
    tariff = other.tariff;
    location = new Point(*other.location);
}

/******************************************************************************
* The function Operation: Taxi destructor - delete the location.
******************************************************************************/
Taxi::~Taxi() {
    delete this->location;
}

/******************************************************************************
* The function Operation: return the taxi id
******************************************************************************/
int Taxi::getId(){
    return this->id;
}

/******************************************************************************
* The function Operation: return the number of steps that the lux taxi do in
* one time passed
******************************************************************************/
int Taxi::getVelocity(){
    return 1;
}

/******************************************************************************
* The function Operation: add given km to the total km passed
******************************************************************************/
void Taxi::addKm(float km){
    this->totalKm += km;
}

/******************************************************************************
* The function Operation: return the taxi's tariff
******************************************************************************/
float Taxi::getTariff(){
    return this->tariff;
}

/******************************************************************************
* The function Operation: the taxi move to next point and add 0.001 km
******************************************************************************/
void Taxi::moveOneStep(Point* point){
    this->updateLocation(point);
    this->addKm(0.001);
}

/******************************************************************************
* The function Operation: update taxi's location
******************************************************************************/
void Taxi::updateLocation(Point* location){
    delete this->location;
    this->location = new Point(*location);
}

/******************************************************************************
* The function Operation: return the taxi's location
******************************************************************************/
Point* Taxi::getLocation(){
    return this->location;
}

/******************************************************************************
* The function Operation: return the taxi's toatalkm passed
******************************************************************************/
float Taxi::getKm(){
    return this->totalKm;
}

/******************************************************************************
* The function Operation: return true if it's the same taxi
******************************************************************************/
bool Taxi::operator==(const Taxi &rhs) const {
    return id == rhs.id;
}

/******************************************************************************
* The function Operation: return true if it's not the same taxi
******************************************************************************/
bool Taxi::operator!=(const Taxi &rhs) const {
    return !(rhs == *this);
}

/******************************************************************************
* The function Operation: serialize a taxi
******************************************************************************/
string Taxi::toString() {
    stringstream str;
    // serialize taxi id
    str << this->id;
    // if lux taxi set type 2 else set 1
    if(this->getVelocity() == 2) str << ",2,";
    else str << ",1,";
    // serialize taxi manufacturer
    switch(this->manufacturer){
        case HONDA:
            str << "H,";
            break;
        case SUBARO:
            str << "S,";
            break;
        case TESLA:
            str << "T,";
            break;
        case FIAT:
            str << "F,";
            break;
        default:
            cout << "no manufacturer exist" << endl;
    }
    // serialize taxi color
    switch(this->color){
        case RED:
            str << "R";
            break;
        case BLUE:
            str << "B";
            break;
        case GREEN:
            str << "G";
            break;
        case PINK:
            str << "P";
            break;
        case WHITE:
            str << "W";
            break;
        default:
            cout << "no color exist" << endl;
    }
    return str.str();
}