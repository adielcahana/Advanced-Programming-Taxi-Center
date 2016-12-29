//
// Created by ori on 28/11/16.
//

#ifndef ASS2_FLOW_H
#define ASS2_FLOW_H

#include "TaxiCenter.h"
#include "Parser.h"
/******************************************************************************
* Flow: handlesthe program flow. this class uses the input parser to
* initialize and update the taxi center.
******************************************************************************/
class Flow {
private:
    Parser parser;
    TaxiCenter* center;
public:
    bool shouldStop;
    Flow();
    ~Flow();
    void initialize();
    void run();
};

#endif //ASS2_FLOW_H
