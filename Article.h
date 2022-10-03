#ifndef SEARCH_ENGINE_ARTICLE_H
#define SEARCH_ENGINE_ARTICLE_H

#include <cstring>
#include <string>
#include <vector>
#include <iostream>

using namespace std;
class Article {
private:
    string ID, title, abstract, body, authors, pdate;
    int wordFrequency;
public:
    Article(string, string, string, string, string);
    //getters
    string getArticleID();
    string getArticleTitle();
    string getArticleAbstract();
    string getArticleBodyText();
    string getArticlesAuthors();
    int getArticleWordFrequency();
    //setters
    void setArticleID(string);
    void setArticleTitle(string);
    void setArticleAbstract(string);
    void setArticleBodyText(string);
    void setArticleAuthors(string);
    void setArticleWordFrequency(int);




};

#endif //SEARCH_ENGINE_ARTICLE_H
