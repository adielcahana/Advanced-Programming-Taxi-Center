#ifndef ASS2_FLOW_H
#define ASS2_FLOW_H

#include "src/TaxiCenter.h"
#include "src/Parser.h"
#include <fstream>
#include "logging/easylogging++.h"

/******************************************************************************
* Flow: handlesthe program flow. this class uses the input parser to
* initialize and update the taxi center.
******************************************************************************/
class TaxiCenterFlow {
private:
    Parser parser;
    TaxiCenter* center;
public:
    bool shouldStop;
    TaxiCenterFlow(int port);
    ~TaxiCenterFlow();
    void initialize();
    void run();
};

#endif //ASS2_FLOW_H
