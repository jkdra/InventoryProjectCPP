//
// Created by Jawad Khadra on 5/5/25.
//

#ifndef ITEM_H
#define ITEM_H

#include "project.h"

using namespace std;

class Item {
protected:
    string name;
    string description;
    int id;

public:
    Item(string name, string description, int id) : name(name), description(description), id(id) {}

    virtual ~Item() = default;

    // Written by Jawad Khadra
    int getID() const {return id;}
    string getName() const {return name;}
    string getDescription() const {return description;}


    void print(ostream& os) const {
        os
        << "ID: " << id << endl
        << "Name: " << name << endl
        << "Description: " << description << endl;
    }
    friend ostream& operator<<(ostream& os, const Item& item) {
        item.print(os);
        return os;
    }
};

class Book : public Item {
protected:
    string title;
    string author;
    string copyrightDate;

public:
    Book(string name, string description, int id, string title, string author, string copyrightDate) : Item(name, description, id), title(title), author(author), copyrightDate(copyrightDate) {}

    // Getters
    string getTitle() const {return title;}
    string getAuthor() const { return author;}
    string getCopyrightDate() const {return copyrightDate;}

    void print(ostream& os) const {
        Item::print(os);
        os
        << "Title: " << title << endl
        << "Author: " << author << endl
        << "Copyright Date: " << copyrightDate << endl;
    }
    friend ostream& operator<<(ostream& os, const Book& book) {
        book.print(os);
        return os;
    }
};

class Magazine : public Item {
protected:
    string edition;
    string title;

    public:
    Magazine(string name, string description, int id, string edition, string title) : Item(name, description, id), edition(edition), title(title) {}
    // Getters
    string getEdition() const {return edition;}
    string getTitle() const {return title;}

    void print(ostream& os) const {
        Item::print(os);
        os
        << "Edition: " << edition << endl
        << "Title: " << title << endl;
    }
    friend ostream& operator<<(ostream& os, const Magazine& magazine) {
        magazine.print(os);
        return os;
    }
};

class Movie : public Item {
    protected:
    string title;
    string director;
    vector<string> mainActors;

public:
    Movie(string name, string description, int id, string title, string director, vector<string> mainActors) : Item(name, description, id), title(title), director(director), mainActors(mainActors) {}
    // Getters
    string getTitle() const {return title;}
    string getDirector() const {return director;}
    vector<string> getMainActors() const {return mainActors;}

    void print(ostream& os) const {
        Item::print(os);
        os
        << "Title: " << title << endl
        << "Director: " << director << endl
        << "Main Actors: " << endl;
        for (auto actor : mainActors) {
            os << actor << endl;
        }
    }
    friend ostream& operator<<(ostream& os, const Movie& movie) {
        movie.print(os);
        return os;
    }
};

#endif //ITEM_H
