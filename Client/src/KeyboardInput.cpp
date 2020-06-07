//
// Created by dorelb@wincs.cs.bgu.ac.il on 07/01/2020.
//

#include <iostream>
#include "../include/KeyboardInput.h"


KeyboardInput::KeyboardInput(ConnectionHandler* connectionHandler, User* user, bool* disconnect): user(user), connectionHandler(connectionHandler), disconnect(disconnect) {}

void KeyboardInput::run() {
    string line;
    unordered_map<string, string> headers;
    Frame* frame;
    int receiptID = 0;

    while (!*disconnect) {
        getline(std::cin, line);
        vector<string> input = splitString(line, " ");
        string command = input[0];

        if(command.compare("join") == 0) {
            //Add the topic to the topic list of the user
            user->addTopic(input[1]);

            //Create a SUBSCRIBE frame
            headers["id"] = to_string(user->getTopicID(input[1]));
            headers["receipt"] = to_string(receiptID);
            headers["destination"] = input[1];

            frame = new Frame("SUBSCRIBE", headers, "");
            user->addReceiptFrame(receiptID, "Joined club " + input[1]);
            receiptID++;
        }

        else if(command.compare("add") == 0) {
            //Create a SEND frame
            headers["destination"] = input[1];
            string bookName = "";
            for (int i = 2; i < (int)input.size() - 1; ++i)
                bookName += input[i] + " ";
            bookName += input[(int)input.size() - 1];

            string body = user->getName() + " has added the book " + bookName;
            frame = new Frame("SEND", headers, body);

            //Add the book to the user's inventory
            user->addBook(bookName, input[1], user->getName());
        }

        else if(command.compare("borrow") == 0) {
            //Create a SEND frame
            headers["destination"] = input[1];
            string bookName = "";
            for (int i = 2; i < (int)input.size() - 1; ++i)
                bookName += input[i] + " ";
            bookName += input[(int)input.size() - 1];

            string body = user->getName() + " wish to borrow " + bookName;
            frame = new Frame("SEND", headers, body);

            //add the book to the user's wish list
            user->addToWishList(bookName);
        }

        else if(command.compare("return") == 0) {
            //Create a SEND frame
            headers["destination"] = input[1];
            string bookName = "";
            for (int i = 2; i < (int)input.size() - 1; ++i)
                bookName += input[i] + " ";
            bookName += input[(int)input.size() - 1];

            string lender = user->getLenderOf(input[1], bookName);
            string body = "Returning " + bookName + " to " + lender;

            frame = new Frame("SEND", headers, body);

            //remove the book from this user's inventory
            user->removeBook(bookName);
        }

        else if(command.compare("status") == 0) {
            //Create a SEND frame
            headers["destination"] = input[1];

            frame = new Frame("SEND", headers, "book status");
        }

        else if(command.compare("exit") == 0) {
            //Create a UNSUBSCRIBE frame
            headers["id"] = to_string(user->getTopicID(input[1]));
            headers["receipt"] = to_string(receiptID);

            frame = new Frame ("UNSUBSCRIBE", headers, "");
            user->addReceiptFrame(receiptID, "Exited club " + input[1]);
            receiptID++;
        }

        else if (command.compare("logout") == 0) {
            //unsubscribe from all of the user's topics
            vector<string> topicsOfUser = user->getIdTopics();
            for (int i = 0; i < (int)topicsOfUser.size(); ++i) {
                //Create a UNSUBSCRIBE frame
                unordered_map<string,string> _headers;
                _headers["id"] = to_string(user->getTopicID(topicsOfUser[i]));
                _headers["receipt"] = to_string(receiptID);

                Frame* unsubFrame = new Frame ("UNSUBSCRIBE", _headers, "");
                user->addReceiptFrame(receiptID, "Exited club " + topicsOfUser[i]);
                receiptID++;

                string s = unsubFrame->toString();
                connectionHandler->sendLine(s);
                delete unsubFrame;
            }

            //Create a DISCONNECT frame
            headers["receipt"] = to_string(receiptID);

            frame = new Frame("DISCONNECT", headers, "");
            user->addReceiptFrame(receiptID, "Logout");
            receiptID++;

            //send the DISCONNECT frame
            string s = frame->toString();
            connectionHandler->sendLine(s);
            delete frame;

            //finish after the end of the logout
            break;
        }

        //send the frame created to the server
        if (!*disconnect) {
            string s = frame->toString();
            connectionHandler->sendLine(s);
            headers.clear();
            delete frame;
        }
    }
}

//split a string according the delimeter
vector<string> KeyboardInput::splitString(string str, string delimiter) {
    vector<string> splittedStr;
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        splittedStr.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    splittedStr.push_back(str);

    return splittedStr;
}
