
#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"
using namespace std;
class CLI {
    DefaultIO* dio;
    map<int, Command*> map_command;
    CommandsHelper *ch;
public:
    explicit CLI(DefaultIO* dio);
    virtual ~CLI();
    void start();
    int getChoiceNum();
    void printMenu();
};

#endif /* CLI_H_ */
