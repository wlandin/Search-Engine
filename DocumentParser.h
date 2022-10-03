
#ifndef SEARCH_ENGINE_DOCUMENTPARSER_H
#define SEARCH_ENGINE_DOCUMENTPARSER_H
#pragma once
#include <iostream>
#include <fstream>
#include "dirent.h"
#include <sys/types.h>
#include "json.hpp"
#include <chrono>
#include <sstream>
#include <unordered_map>
#include <algorithm>

#include "IndexHander.h"
#include "Article.h"

#include "HashTable.h"
extern std::unordered_map<std::string,std::string> stopwords;

using json = nlohmann::json;
class DocumentParser {
public:
    std::vector<std::string> files, emptyVector, searchVector;
    std::vector<Article> articles;
    IndexHander handler;
    std::vector<std::vector<std::string>> searchResuts;
    HashTable<std::string, std::string> authorHash; //it is a type string, vector<string> hash table with the last names of the authors and the id they are found in.
    bool resultFound=false;

    const char *filepath;
    DocumentParser();

    void openFile();
    void readFile();

    Word getWordFromString(std::string&);

    void processQuery(std::string&);
    void process1Query(std::vector<std::string>&);
    void processOrQuery(std::vector<std::string>&);
    void processANDQuery(std::vector<std::string>&);
    void displayOrResults(std::vector<std::string>&);
    std::vector<std::string> createQueryVector(std::string&);
    void readStopWords();

    void parseFoundArticles(std::vector<Article>&, std::string&);
    void printFirst300Words(Article&);

    Article findArticleFromID(std::string&);

};


#endif //SEARCH_ENGINE_DOCUMENTPARSER_H
