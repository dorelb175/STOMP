//
// Created by dorelb@wincs.cs.bgu.ac.il on 07/01/2020.
//

#include <vector>
#include "../include/Frame.h"
#include "../include/KeyboardInput.h"

Frame::Frame(string command, unordered_map<string, string> headers, string body): command(command), headers(headers), body(body) {}

string Frame::getCommand() {
    return command;
}

unordered_map<string, string> Frame::getHeaders() {
    return headers;
}

string Frame::getBody() {
    return body;
}

void Frame::addHeader(string header, string value) {
    headers[header] = value;
}

void Frame::setBody(string body) {
    this->body = body;
}

string Frame::toString() {
    string str = command;
    str.append( "\n" );

    if (!headers.empty()) {
        for (auto it = headers.begin(); it != headers.end(); ++it) {
            string key = it->first;
            string value = it->second;
            str.append( key );
            str.append( ":" );
            str.append( value );
            str.append( "\n" );
        }
    }
    str.append( "\n" );

    if (body.compare("") != 0)
        str.append( body );

    return str;
}

Frame Frame::toFrame(string str) {
    vector<string> parts = KeyboardInput::splitString(str, "\n");
    string command = parts[0];
    unordered_map<string, string> _headers;
    string body = "";

    //MESSAGE and ERROR frame always have body
    if ((command.compare("MESSAGE") == 0) | (command.compare("ERROR") == 0)) {
        //Body:
        int bodyIndex = -1;
        for (int i = (int) parts.size() - 1; bodyIndex == -1 && i > 0; i--) {
            if (parts[i].compare("") != 0) {
                bodyIndex = i;
                body = parts[i];
            }
        }

        bool empty = false;
        //headers:
        for (int i = 1; !empty && i < bodyIndex; i++) {
            if (parts[i].compare("") != 0) {
                vector<string> headerParts = KeyboardInput::splitString(parts[i], ":");
                _headers[headerParts[0]] = headerParts[1];
            }
            else
                empty = true;
        }
    }
    else {
        //headers:
        bool empty = false;
        for (int i = 1; !empty && i < (int) parts.size(); i++) {
            if (parts[i].compare("") != 0) {
                vector<string> headerParts = KeyboardInput::splitString(parts[i], ":");
                _headers[headerParts[0]] = headerParts[1];
            }
            else
                empty = true;
        }
    }

    return Frame(command, _headers, body);
}