#ifndef ASS2_LUXTAXI_H
#define ASS2_LUXTAXI_H

#include "Driver.h"

/******************************************************************************
* LuxTaxi: like regular taxi but move twice faster
******************************************************************************/
//class LuxTaxi: public Taxi {
////    friend class boost::serialization::access;
//    template<class Archive>
////    void serialize(Archive &ar, const unsigned int version)
////    {
////        ar & boost::serialization::base_object<Taxi>(*this);
////    }
//public:
//    LuxTaxi(int id, Manufacturer type, Color color):
//            Taxi(id, type, color, 2){};
////    LuxTaxi(){};
//    int getVelocity();
//};
class LuxTaxi: public Taxi {
private:
public:
    LuxTaxi(int id, Manufacturer type, Color color):
            Taxi(id, type, color, 2){};
    int getVelocity();
};

#endif //ASS2_LUXTAXI_H
