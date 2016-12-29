#include "Flow.h"

int main(){
    Flow flow;
    while (!flow.shouldStop) {
        flow.initialize();
        if (!flow.shouldStop) flow.run();
    }
}