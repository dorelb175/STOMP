//
// Created by dorelb@wincs.cs.bgu.ac.il on 07/01/2020.
//

#ifndef CLIENT_BOOK_H
#define CLIENT_BOOK_H

#include <string>
#include <vector>

using namespace std;
class User;

class Book {
private:
    string name;
    string topic;
    string owner;
    string borrower;
    bool borrowerIsOwner;
public:
    Book(string name, string topic, string owner, string borrower);
    string getName();
    string getOwner();
    string getBorrower();
    void setBorrower(string borrower);
    void setOwner(string owner);
    bool getBorrowerIsOwner();
    void setBorrowerIsOwner(bool value);
};

#endif //CLIENT_BOOK_H
