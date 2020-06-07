//
// Created by dorelb@wincs.cs.bgu.ac.il on 07/01/2020.
//

#ifndef CLIENT_INVENTORY_H
#define CLIENT_INVENTORY_H

#include <vector>
#include <unordered_map>
#include "Book.h"
#include "Topic.h"

class User;

using namespace std;

class Inventory {
private:
    unordered_map<string, Topic*> topics;

public:
    Inventory();
    unordered_map<string, Topic*> getTopics();
    void addBook(string topic, Book* book);
    void lendBook(Book* book, string borrower);
    bool containsBook(string topic, string bookName);
    void removeBook(string bookName);
    string getBooksOfTopic(string topic);
    void cleanInventory();
    void setBorrowerIsOwner(string topic, string bookName, bool value);
    string getLenderOf(string topic, string bookName);
};


#endif //CLIENT_INVENTORY_H