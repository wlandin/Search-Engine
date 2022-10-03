
#include "Word.h"

Word::Word() {
    word="";
}

Word::Word(std::string &s, std::vector<std::string> &v) {
    word = s;
    ids = v;
}
Word::Word(std::string &s, std::string &id) {
    word = s;
    ids.push_back(id);
    counter =0;
}

void Word::setString(std::string &s) {
    word =s;
}

std::string Word::getString() const {
    return word;
}

void Word::addID(std::string &id) {
    ids.push_back(id);
}

std::vector<std::string> Word::getIDs() const {
    return this->ids;
}

void Word::displayWord() {
    std::cout<<"the word \""<<this->word<<"\" is found in:\n";

    for(int i=0; i<ids.size(); i++){
        std::cout<<ids[i]<<"\n" ;
    }
}
void Word::increaseCounter(){
    this->counter++;
}

int Word::getCount(){
    return counter;
}

std::ostream &operator<<(std::ostream &os, const Word &w) {
    os<<"AVLWORD\n";
    os<<w.word<<"\n";
    for(int i=0; i<w.ids.size(); i++){
        os<<w.ids[i]<<"\n";
    }
    os<<"\n";
    return os;
}
//virus, aaab1 aaab2


bool Word::operator<(const Word &rhs) const {
    return word<rhs.word;
}


bool Word::operator>(Word &rhs) {
    return word>rhs.word;
}

bool Word::operator==(Word &rhs) {
    return word==rhs.word;
}

bool Word::operator<(std::string &rhs) const {
    return word<rhs;
}

bool Word::operator>(std::string &rhs) {
    return word>rhs;
}

bool Word::operator==(std::string &rhs) {
    return word==rhs;
}
Word& Word::operator+(const Word&rhs){
    if(ids[ids.size()-1] != rhs.ids[0]){
        ids.push_back(rhs.ids[0]);
    }
    return *this;
}

Word Word::getWordfromString(std::string &s) {
    return Word();
}



