#ifndef SEARCH_ENGINE_INDEXHANDER_H
#define SEARCH_ENGINE_INDEXHANDER_H
#pragma once
#include "Word.h"
#include "porter2_stemmer.h"
#include "AVLTree.h"
#include <algorithm>
#include <unordered_map>
#include <set>
#include <fstream>
#include <sstream>
extern std::unordered_map<std::string,std::string> stopwords;
extern std::vector<std::string>emptyVector;

using Porter2Stemmer::stem;
class IndexHander {
public:

    IndexHander();

    void checkToAddWord(std::string &w, std::string &FileId);
    AVLTree<Word> allindex;
    std::vector<Word> allindexVector;

    void printExample(std::vector<Word>);
    std::string remPunc(std::string&);

    int treeCounter=0;
    int wordCounter =0;

    bool stringFind(std::string&);

    void printTreeToFile();
    void readInIndex();

    void printHashtoFile();
    void readInHash();

};


#endif //SEARCH_ENGINE_INDEXHANDER_H
