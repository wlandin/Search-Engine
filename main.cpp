#include <iostream>
#include "UserInterface.h"
#include <unordered_map>
std::unordered_map<std::string,std::string> stopwords;
std::vector<std::string> emptyVector;


//using namespace std;
//monkey /mnt/c/Users/wlcoo/Programs/21s-srch-eng-team-landin/timing_demo_data_sets/ds_03/
int main(int argc, char**argv) {
    if (argc > 1) {
        //Run Program
//        std::chrono::time_point<std::chrono::high_resolution_clock> start, end; //timer for parsing speed
//        start = std::chrono::high_resolution_clock::now(); //start clock
        UserInterface ui;
        ui.homeMenu();
//        end = std::chrono::high_resolution_clock::now(); //end clock
//        std::chrono::duration<double> time_in_seconds = end - start;
//        std::cout << std::fixed << "\nDuration: " << time_in_seconds.count() << std::endl;

        //query.search();

    } else {
        //Run Catch tests
        //Catch::Session().run();
        std::cout<<"Hey buddy try this\n";
        HashTable<std::string, std::string> EXHash;
        std::string author1 = "Donna";
        std::string author2 ="Kyle";
        std::vector<std::string> hashVector;
        std::string id1 = "a21a";
        std::string id2 = "a21b";
        EXHash.insert(author1,id1);
        EXHash.insert(author2,id2);
        hashVector = EXHash.getValueFromKey(author1);
        //EXHash.insert()
        std::cout<<"Printing hashVector\n";
        for(int i=0; i<hashVector.size(); i++){
            std::cout<<hashVector[i]<<"\n";
        }


//        AVLTree<int> treee;
//        int nine = 9;
//        //treee.insert(eight);
//        for(int i=0; i<10; i++){
//            treee.insert(i);
//        }
//        treee.insert(nine);
//        treee.treedisplay();
//        int negativetwo = -2;
//        int two = 2;
//        if(treee.isFound(negativetwo)){
//            std::cout<<"It somehow found negative 2";
//        }else{
//            std::cout<<"It worked\n";
//        }
//
//        //std::cout << "Time for some Amazing Coding!" << std::endl;
//        std::string virus = "virus!!!";
//        std::vector<std::string> EXIds;
//        std::string id1 = "ffffff";
//        std::string id2 = "fffffe";
//        //Word exampleWord(virus,EXIds);
//        Word exampleWord(virus, id1);
//        exampleWord.displayWord();
//        Word exampleWord2(virus,id2);
//        exampleWord = exampleWord+exampleWord2;
//        exampleWord.displayWord();
//        exampleWord.displayWord();
//        exampleWord.addID(id1);
//        exampleWord.displayWord();
//        exampleWord.addID(id2);
//        exampleWord.displayWord();
//        exampleWord.setWord("Corona!");
//        exampleWord.displayWord();
//        std::vector<std::string> idcopy = exampleWord.getIDs();
//        for(int i=0; i<idcopy.size(); i++){
//            std::cout<<idcopy[i]<<" index number:"<<i<<"\n";
//        }
//        std::string test = exampleWord.getWord();
//        std::cout<<"Yay example time: "<<test<<"\n";


    }

    return 0;
}