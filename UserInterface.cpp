
#include "UserInterface.h"

UserInterface::UserInterface() {}


void UserInterface::startQuery() {
    bool AndFlag, OrFlag, AuthorFlag, NotFlag; //make flags for AND, NOT, OR, or AUTHOR
    AndFlag=OrFlag=AuthorFlag=NotFlag=false; //set them all equal to false

    //OR monkey NOT virus
    std::string searchquery;
    std::vector<std::string> searchQueryV;
    std::cout<<"Please enter a word or words to search, use AND or OR at the beginning of your search ";
    std::cin.ignore();
    getline(std::cin, searchquery);
    //std::cin>>searchquery;
    //while(searchquery!="exit"){
        std::cout<<"You entered: \""<<searchquery<<"\" checking...\n";
        std::istringstream ss(searchquery);
        while(ss>>searchquery){
            p.searchVector.push_back(searchquery);
        }
        //searchQueryV= p.createQueryVector(searchquery);
        if(p.searchVector[0]=="AND"){
            AndFlag=true;
        }
        if(p.searchVector[0]=="OR"){
            OrFlag=true;
        }
// good search words nephropathogenic
        if(AndFlag){
            std::cout<<"An \"AND\" was found\n";
            p.processANDQuery(p.searchVector);
        }else if(OrFlag){
            std::cout<<"An \"OR\" was found, execting or query\n";
            p.processOrQuery(p.searchVector);
        }else{
            p.process1Query(p.searchVector);

        }

        //p.processQuery(searchquery);
        //std::cout<<"Please enter a word you would like to search(Enter \"exit\" to end program) ";
        //std::cin>>searchquery;
    //}
    std::string repeat;
    std::cout<<"Would you like to search again? (y/n) ";
        std::cin>>repeat;
        if(repeat=="y"){
            p.searchVector.clear(); //clear the search vector
            startQuery();
        }else if(repeat=="n"){
            std::cout<<"redirecting to home menu... \n";
            homeMenu();
        }
}

void UserInterface::run() {
    p.readStopWords();
    p.openFile();
    p.readFile();
}

void UserInterface::homeMenu() { //self explanatory user interface
    std::string userInput;
    std::cout<<"Welcome to the Team Landin search engine, please insert a number to continue.\n";
    std::cout<<"\t1. Index Creation\n"<<"\t2. Index management\n"<<"\t3. Exit Program\n";
    std::cin>>userInput;
    if(userInput=="1"){
        std::cout<<"please insert a number to continue\n";
        std::cout<<"\t1. Create New Index and Search!\n"<<"\t2. Read in Index and Search\n"<<"\t3. Search current Index\n"<<"\t4. Return to Main Menu\n";
        std::cin>>userInput;
        if(userInput=="1"){
            std::cout<<"Creating New Index please wait... \n";
            run();
            std::cout<<"Would you like to search? (y/n)\n";
            std::cin>>userInput;
            if(userInput=="y"){
                startQuery();
            }else{
                homeMenu();
            }
        }else if(userInput=="2"){
            std::cout<<"Reading in Index, and preparing search please wait...\n";
            p.handler.readInIndex();
            std::cout<<"Would you like to print? (y/n)\n";
            std::cin>>userInput;
            if(userInput=="y"){
                p.handler.allindex.treedisplay();
                homeMenu();
            }else{
                homeMenu();
            }

        }else if(userInput=="3"){
            std::cout<<"Preparing search please wait...\n";
            startQuery();
        }else if(userInput=="4"){
            std::cout<<"\nRedirecting...\n";
            homeMenu();
        }else{
            std::cout<<"Please enter a valid input, redirecting...\n";
            homeMenu();
        }
    } else if(userInput=="2"){
        std::cout<<"please insert a number to continue\n";
        std::cout<<"\t1. Clear Index\n"<<"\t2. Print Index to file\n"<<"\t3. Return to main menu\n";
        std::cin>>userInput;
        if(userInput=="1"){
            std::cout<<"Clearing index... \n";
            p.handler.allindex.makeEmpty();
            homeMenu();
        }else if(userInput=="2"){
            p.handler.printTreeToFile();
            homeMenu();
        }else if(userInput=="3"){
            std::cout<<"\nThank you for using this program, Goodbye!\n";
            homeMenu();
        }else{
            std::cout<<"Please enter a valid input, redirecting...\n";
            homeMenu();
        }
    }else if(userInput=="3"){
        std::cout<<"\nThank you for using this program, Goodbye!\n";
    }else{
        std::cout<<"Please enter a valid input, redirecting...\n";
        homeMenu();
    }
}
