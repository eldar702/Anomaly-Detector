#include "CLI.h"
#include <string.h>
/** constructor: */
CLI::CLI(DefaultIO* rio) {
    this->dio = rio;
    this->ch = new CommandsHelper();
    map_command[1] =  new Upload_csv(dio,ch);
    map_command[2] = new changeThreshold(dio, ch);
    map_command[3] = new detectAnomalies(dio, ch);
    map_command[4] = new displayResults(dio, ch);
    map_command[5] = new analyzeResults(dio, ch);

/*    this->commands = new Command * [commands_number];
    this->commands[0] = new Upload_csv(dio,ch);
    this->commands[1] = new changeThreshold(dio, ch);
    this->commands[2] = new detectAnomalies(dio, ch);
    this->commands[3] = new displayResults(dio, ch);
    this->commands[4] = new analyzeResults(dio, ch);*/
}
// distructor:
CLI::~CLI() {
    for (int i = 1; i <= map_command.size(); ++i)
        delete this->map_command[i];
    delete ch;
}

/** print the menu */
void CLI::printMenu() {
    dio->write("Welcome to the Anomaly Detection Server.\n");
    dio->write("Please choose an option:\n");
    for (int i = 1; i <= map_command.size(); ++i)
        dio->write(this->map_command[i]->getDescription() + "\n");
    dio->write("6.exit\n");
}
/** function to start the program */
void CLI::start() {
    int choice_number = 0;
    while (choice_number != this->map_command.size() + 1) {
        printMenu();
        choice_number = getChoiceNum();
        if (choice_number == this->map_command.size() + 1) {
            continue;
        }
        this->map_command[choice_number]->execute();
    }
}
/** function to get the choice number from the user, so later on we will run the wanted command */
int CLI::getChoiceNum() {
    float temp;
    float *f = &temp;
    this->dio->read(f);
    // check if the given number is legal number (that is an integer number between 1 to the commands number)
    if( (temp != int(temp))  || (temp <= 0) || (temp > map_command.size() + 1)) {
        return getChoiceNum();
    }
    else
        return temp;
    return temp;
}

