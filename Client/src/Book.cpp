//
// Created by dorelb@wincs.cs.bgu.ac.il on 07/01/2020.
//

#include "../include/Book.h"

Book::Book(string name, string topic, string owner, string borrower): name(name), topic(topic), owner(owner), borrower(borrower), borrowerIsOwner(true) {}

string Book::getName() {
    return name;
}

string Book::getOwner() {
    return owner;
}

string Book::getBorrower() {
    return borrower;
}

void Book::setBorrower(string borrower) {
    this->borrower = borrower;
}

void Book::setOwner(string owner) {
    this->owner = owner;
}

bool Book::getBorrowerIsOwner() {
    return borrowerIsOwner;
}

void Book::setBorrowerIsOwner(bool value) {
    borrowerIsOwner = value;
}