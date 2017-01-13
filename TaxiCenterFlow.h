#ifndef ASS2_FLOW_H
#define ASS2_FLOW_H

#include "src/TaxiCenter.h"
#include "src/Parser.h"
#include <fstream>

#ifndef CAN_CONTINUE
#define CAN_CONTINUE
namespace global {
    bool can_continue = false;
}
#endif
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
