//
// Created by dorelb@wincs.cs.bgu.ac.il on 07/01/2020.
//

#include "../include/Inventory.h"
#include <iostream>

Inventory::Inventory(): topics() {}

unordered_map<string, Topic *> Inventory::getTopics() {
    return topics;
}

void Inventory::addBook(string topic, Book *book) {
    if (topics[topic] == nullptr)
        topics[topic] = new Topic(topic);
    topics[topic]->addBook(book);
}

void Inventory::lendBook(Book *book, string borrower) {
    book->setBorrower(borrower);
}

bool Inventory::containsBook(string topic, string bookName) {
    if (topics[topic] == nullptr)
        return false;

    return topics[topic]->containsBook(bookName).compare("") != 0;
}

void Inventory::removeBook(string bookName) {
    for (auto it = topics.begin(); it != topics.end(); ++it) {
        //the book is in the current topic
        string topic = it->second->containsBook(bookName);
        if (topic.compare("") != 0)
            topics[topic]->removeBook(bookName);
    }
}

string Inventory::getBooksOfTopic(string topic) {
    if (topics[topic] == nullptr)
        return "";

    return topics[topic]->getBooksOfTopic();
}

void Inventory::cleanInventory() {
    for (auto it = topics.begin(); it != topics.end(); ++it) {
        it->second->cleanTopic();
        delete it->second;
    }
    topics.clear();
}

void Inventory::setBorrowerIsOwner(string topic, string bookName, bool value) {
    topics[topic]->setBorrowerIsOwner(bookName, value);
}

string Inventory::getLenderOf(string topic, string bookName) {
    return topics[topic]->getLenderOf(bookName);
}
