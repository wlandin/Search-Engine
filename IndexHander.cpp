
#include "IndexHander.h"

IndexHander::IndexHander() {}

void IndexHander::checkToAddWord(std::string &w, std::string &FileId) {
    //check if the word has more than 1 characters in it, to ignore random .s
    //w = monkeys
    if(w.size()>1){
        w = remPunc(w);
        transform(w.begin(),w.end(),w.begin(),::tolower);//found this on tutorialspoint, makes use of the transform function for cstring
        if(stopwords.find(w)==stopwords.end()){ //Check to see if the string is a stopword
            Porter2Stemmer::stem(w);
            //The word is not a stopword
            if(stringFind(w)){ //this was for the purpose of testing if every words was found as a duplicate
                Word wordobj(w,FileId);
                allindex.insert(wordobj);//the insert function is called but the duplicate word gets added onto the vector
            }else{
                //if the word is not found in the tree, increase the tree counter to count how many items are in the tree, and insert the new item into the tree
                Word wordobj(w,FileId);
                treeCounter++;
                wordCounter++;
                allindex.insert(wordobj);
                //1,940,073 words with stemmer
                //27704
            }
        }else{
            //The word is a stopword, do nothing

        }

        //allindexVector.push_back(wordobj);

        //allindex.insert(wordobj);
    }
    //printExample(allindexVector);
}

//16441 in ds_01  13176 are duplicated words
//373728 in ds_02  350026 are duplicated words
//4595233 in ds_03  4479197 are duplicated words
void IndexHander::printExample(std::vector<Word> v) {
    for(int i=0; i<v.size(); i++){
        v[i].displayWord();
    }
}

std::string IndexHander::remPunc( std::string& word){
    std::string result;
    for (auto ch: word)
        if (!ispunct(ch))
            result += ch;
    return result;
}

bool IndexHander::stringFind(std::string& s) {
    Word tempWord(s,emptyVector);
    return allindex.isFound(tempWord);
}

void IndexHander::printTreeToFile() {
    std::ofstream output("tree100000.txt");
    allindex.indexFilePrint(output);
}



void IndexHander::readInIndex() { //Does not work

    std::ifstream input("tree10002.txt");
    std::string AVLtoken, word, ids;
    while(input>>AVLtoken){
        if(AVLtoken=="AVLWORD"){
            input>>word;
            while(input>>AVLtoken){
//                if(AVLtoken=="AVLWORD"){
//                    break;
//                }
                checkToAddWord(word,AVLtoken);
            }
        }
    }

}

void IndexHander::printHashtoFile() {
    std::ofstream output("hash1000");

}




