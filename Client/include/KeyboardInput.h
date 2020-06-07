//
// Created by dorelb@wincs.cs.bgu.ac.il on 07/01/2020.
//

#ifndef CLIENT_KEYBOARDINPUT_H
#define CLIENT_KEYBOARDINPUT_H

#include <string>
#include <vector>
#include "Frame.h"
#include "User.h"
#include "connectionHandler.h"

using namespace std;

class KeyboardInput{
private:
    User* user;
    ConnectionHandler* connectionHandler;
    bool* disconnect;
public:
    KeyboardInput(ConnectionHandler* connectionHandler, User* user, bool* disconnect);
    void run();
    static vector<string> splitString(string str, string delimiter);
};

#endif //CLIENT_KEYBOARDINPUT_H
