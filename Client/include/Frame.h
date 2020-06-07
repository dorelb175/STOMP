//
// Created by dorelb@wincs.cs.bgu.ac.il on 07/01/2020.
//

#ifndef CLIENT_FRAME_H
#define CLIENT_FRAME_H

#include <string>
#include <unordered_map>
using namespace std;

class Frame{
private:
    string command;
    unordered_map<string, string> headers;
    string body;

public:
    Frame(string command, unordered_map<string, string> headers, string body);
    string getCommand();
    unordered_map<string, string> getHeaders();
    string getBody();
    void addHeader(string header, string value);
    void setBody (string body);
    string toString();
    static Frame toFrame(string str);
};

#endif //CLIENT_FRAME_H
