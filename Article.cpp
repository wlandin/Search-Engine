
#include "Article.h"

Article::Article(string id, string t, string ab, string bt, string auth) {
    ID = id;
    title =t;
    abstract = ab;
    body = bt;
    authors = auth;
    wordFrequency=0;
}
//getters
string Article::getArticleID() {
    return ID;
}

string Article::getArticleTitle() {
    return title;
}

string Article::getArticleAbstract() {
    return abstract;
}

string Article::getArticleBodyText() {
    return body;
}

string Article::getArticlesAuthors() {
    return authors;
}
int Article::getArticleWordFrequency() {
    return wordFrequency;
}


//setters
void Article::setArticleID(string i) {
    ID =i;
}

void Article::setArticleTitle(string t) {
    title =t;
}

void Article::setArticleAbstract(string a) {
    abstract = a;
}

void Article::setArticleBodyText(string b) {
    body = b;
}

void Article::setArticleAuthors(string auth) {
    authors = auth;
}
void Article::setArticleWordFrequency(int f) {
    wordFrequency =f;
}

