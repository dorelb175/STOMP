//
// Created by dorelb@wincs.cs.bgu.ac.il on 08/01/2020.
//

#include <iostream>
#include "../include/KeyboardInput.h"
#include <thread>

void close();

using namespace std;

int main(int argc, char *argv[]) {
    //--------------------------CONNECT TO SERVER--------------------------
    cout << "Please login: \n";
    string line;
    getline(std::cin, line);

    //split the input from the keyboard
    vector<string> input = KeyboardInput::splitString(line, " ");
    string command = input[0];

    //The login command should come first
    while (command.compare("login") != 0) {
        cout << "Login First! \n";
        string line;
        getline(std::cin, line);
        input = KeyboardInput::splitString(line, " ");
        command = input[0];
    }
    string host = "";
    short port = 0;

    if (command.compare("login") == 0) {
        //Split the host-port "word"
        vector<string> host_port = KeyboardInput::splitString(input[1], ":");
        host = host_port[0];
        port = stoi(host_port[1]);
    }
    ConnectionHandler connectionHandler(host, port);

    if (!connectionHandler.connect()) {
        std::cerr << "Socket Error: Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
    //Create a CONNECT frame
    unordered_map<string, string> headers;
    headers["login"] = input[2];
    headers["passcode"] = input[3];
    headers["host"] = host;
    headers["accept-version"] = "1.2";

    //send the CONNECT frame to the server
    Frame* frame = new Frame("CONNECT", headers, "");
    string s = frame->toString();
    connectionHandler.sendLine(s);
    delete frame;

    //Create the User object for this client
    User* user = new User(input[2], input[3]);

    //----------------------END CONNECTING TO SERVER----------------------

    bool* disconnect = new bool(false);

    //Create and run the thread of the keyboard
    KeyboardInput keyboardInput(&connectionHandler, user, disconnect);
    thread keyboardThread(&KeyboardInput::run, &keyboardInput);

    //-------------------------Thread of the socket-------------------------
    string answer;

    while (!*disconnect) {
        answer = "";
        if (!connectionHandler.getLine(answer)) {
            cout << "couldn't receive message from server. Exiting...\n" << endl;
            *disconnect = true;
        }

        Frame frame = Frame::toFrame(answer);
        string command = frame.getCommand();

        if (command.compare("CONNECTED") == 0) {
            cout << "Login successful\n";
        }
        else if (command.compare("RECEIPT") == 0) {
            int receiptId = stoi(frame.getHeaders()["receipt-id"]);
            unordered_map<int,string> receiptFramesMap = user->getReceiptMap();
            if (receiptFramesMap[receiptId].compare("") != 0) {
                //Receipt of SUBSCRIBE/UNSUBSCRIBE frame
                if (receiptFramesMap[receiptId].compare("Logout") != 0)
                    cout << receiptFramesMap[receiptId] << "\n";
                //Receipt of DISCONNECT frame
                else {
                    *disconnect = true;
                    connectionHandler.close();
                    cout << "Disconnected from server Successfully\n";
                }
            }
        }
        else if (command.compare("MESSAGE") == 0) {
            string topic = frame.getHeaders()["destination"];

            //split the body of the frame received
            vector<string> body = KeyboardInput::splitString(frame.getBody(), " ");
            cout << topic << ": " << frame.getBody() << "\n";
            string bookName = "";

            //a borrow request message
            if (frame.getBody().find("borrow") != std::string::npos) {
                for (int i = 4; i < (int)body.size() - 1; ++i)
                    bookName += body[i] + " ";
                bookName += body[(int)body.size() - 1];

                if (user->containsBook(topic, bookName)) {
                    //send a SEND frame to the server
                    unordered_map<string,string> _headers;
                    _headers["destination"] = topic;
                    Frame* sendFrame = new Frame("SEND", _headers, user->getName() + " has " + bookName);
                    string s = sendFrame->toString();
                    connectionHandler.sendLine(s);
                    delete sendFrame;
                }
            }
            //someone has the book that was requested by the user
            else if (frame.getBody().find("added") == std::string::npos && frame.getBody().find("has") != std::string::npos) {
                for (int i = 2; i < (int)body.size() - 1; ++i)
                    bookName += body[i] + " ";
                bookName += body[(int)body.size() - 1];

                //this user wished to borrow this book
                if (user->inWishList(bookName)) {
                    string lender = body[0];
                    //remove the book from the wish list
                    user->removeFromWishList(bookName);
                    //add the book to the user's inventory and "take" it from the lender
                    user->addBook(bookName, topic, lender);

                    //send a SEND frame to the server: Taking book from lender...
                    unordered_map<string,string> _headers;
                    _headers["destination"] = topic;
                    Frame* sendFrame = new Frame("SEND", _headers, "Taking " + bookName + " from " + lender);
                    string s = sendFrame->toString();
                    connectionHandler.sendLine(s);
                    delete sendFrame;
                }
            }
            //someone took a book
            else if ((frame.getBody().find("Taking") != std::string::npos) | (frame.getBody().find("taking") != std::string::npos)) {
                string lender = body[(int)body.size() - 1];

                for (int i = 1; i < (int)body.size() - 3; ++i)
                    bookName += body[i] + " ";
                bookName += body[(int)body.size() - 3];

                //the book was borrowed from this user
                if (user->getName().compare(lender) == 0) {
                    //remove the book from this user's inventory
                    user->setBorrowerIsOwner(topic, bookName, false);
                }
            }
            //someone is returning a book
            else if ((frame.getBody().find("Returning") != std::string::npos) | (frame.getBody().find("returning") != std::string::npos)) {
                string lender = body[(int)body.size() - 1];

                for (int i = 1; i < (int)body.size() - 3; ++i)
                    bookName += body[i] + " ";
                bookName += body[(int)body.size() - 3];

                //the book was borrowed from this user
                if (user->getName().compare(lender) == 0) {
                    //add the book to the lender's inventory
                    user->setBorrowerIsOwner(topic, bookName, true);
                }
            }
            //book status
            else if (frame.getBody().find("status") != std::string::npos) {
                //send a SEND frame to the server with the books of this user in the topic
                unordered_map<string,string> _headers;
                _headers["destination"] = topic;
                Frame* sendFrame = new Frame("SEND", _headers, user->getName() + ":" + user->getBooksOfTopic(topic));
                string s = sendFrame->toString();
                connectionHandler.sendLine(s);
                delete sendFrame;
            }
        }
        else if (command.compare("ERROR") == 0) {
            cout << frame.getBody() << "\n";
            *disconnect = true;
            connectionHandler.close();
            cout << "Disconnecting\n";
        }
    }

    //-------------------End of Thread of the socket-------------------------

    keyboardThread.join();
    user->cleanInventory();
    delete user;
    delete disconnect;
    return 0;
}