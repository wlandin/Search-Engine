#ifndef SEARCH_ENGINE_WORD_H
#define SEARCH_ENGINE_WORD_H

#pragma once
#include <cstring>
#include <string>
#include <vector>
#include <iostream>


class Word {
public:
    std::string word;
    std::vector<std::string> ids;
    int counter;

    Word();

    Word(std::string&,std::vector<std::string>&);
    Word(std::string&,std::string&);

    void setString(std::string &s);
    std::string getString() const;

    void addID(std::string &);
    std::vector<std::string> getIDs() const;

    Word getWordfromString(std::string &s);

    void displayWord();

    void increaseCounter();

    int getCount();

    friend std::ostream& operator<< (std::ostream&, const Word&);

    //operators for comparison to word
    bool operator<(const Word &rhs) const;
    bool operator>( Word &rhs);
    bool operator==( Word &rhs);
    //operators for comparison to string
    bool operator<(std::string &rhs) const;
    bool operator>(std::string &rhs);
    bool operator==(std::string &rhs);
    Word& operator+(const Word &rhs);

};


#endif //SEARCH_ENGINE_WORD_H
