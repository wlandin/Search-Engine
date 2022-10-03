
#ifndef SEARCH_ENGINE_USERINTERFACE_H
#define SEARCH_ENGINE_USERINTERFACE_H
#include "DocumentParser.h"

class UserInterface {
private:
    DocumentParser p;
    IndexHander h;
public:
    UserInterface();

    void run();
    void startQuery();
    void homeMenu();


};


#endif //SEARCH_ENGINE_USERINTERFACE_H
