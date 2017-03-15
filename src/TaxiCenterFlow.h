#ifndef ASS2_FLOW_H
#define ASS2_FLOW_H

#include "Taxi-Center/TaxiCenter.h"
#include "comunication/GuiProtocol.h"
#include "general/Parser.h"
#include <fstream>

/******************************************************************************
* Flow: handlesthe program flow. this class uses the input parser to
* initialize and update the taxi center.
******************************************************************************/
class TaxiCenterFlow: public Server {
private:
    Parser parser;
    TaxiCenter* center;
public:
    bool shouldStop;
    TaxiCenterFlow(Tcp* tcp);
    ~TaxiCenterFlow();
    void run();
};

#endif //ASS2_FLOW_H
