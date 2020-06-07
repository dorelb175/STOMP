#include "../include/User.h"


User::User(string username, string passcode) : username(username), passcode(passcode), id_topics(), receiptMap(), inventory(), booksWishList(), isConnected(true) {}

void User::addTopic(string topic) {
    if (getTopicID(topic) == -1)
        id_topics.push_back(topic);
}

int User::getTopicID(string topic) {
    int pos = -1;
    bool found = false;
    for (int i = 0; !found && i < (int)id_topics.size(); ++i) {
        if (id_topics[i].compare(topic) == 0) {
            pos = i;
            found = true;
        }
    }
    return pos;
}

string User::getName() {
    return username;
}

vector<string> User::getIdTopics(){
    return id_topics;
}

void User::addReceiptFrame(int receiptID, string s) {
    receiptMap[receiptID] = s;
}

unordered_map<int, string> User::getReceiptMap() {
    return receiptMap;
}

void User::addBook(string bookName, string topic, string owner) {
    Book* book = new Book(bookName, topic, owner, this->getName());
    inventory.addBook(topic, book);
}


bool User::containsBook(string topic, string bookName) {
    bool b = inventory.containsBook(topic, bookName);
    return b;
}

void User::addToWishList(string bookName) {
    booksWishList.push_back(bookName);
}

void User::removeFromWishList(string bookName) {
    for (auto it = booksWishList.begin(); it != booksWishList.end(); ++it) {
        if (it->compare(bookName) == 0) {
            booksWishList.erase(it);
            return;
        }
    }
}

bool User::inWishList(string bookName) {
    bool found = false;
    for (int i = 0; !found && i < (int)booksWishList.size(); ++i)
        if (booksWishList[i].compare(bookName) == 0)
            found = true;
    return found;
}

void User::removeBook(string bookName) {
    inventory.removeBook(bookName);
}

string User::getBooksOfTopic(string topic) {
    return inventory.getBooksOfTopic(topic);
}

void User::cleanInventory() {
    inventory.cleanInventory();
}

void User::setBorrowerIsOwner(string topic, string bookName, bool value) {
    inventory.setBorrowerIsOwner(topic, bookName, value);
}

string User::getLenderOf(string topic, string bookName) {
    return inventory.getLenderOf(topic, bookName);
}


