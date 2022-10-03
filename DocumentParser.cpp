
#include "DocumentParser.h"

DocumentParser::DocumentParser() {}

void DocumentParser::openFile() {
//    std::cout << "starting openFile function" << std::endl;
    //filepath = "/mnt/c/Users/wlcoo/Programs/21s-srch-eng-team-landin/timing_demo_data_sets/ds_03/";
    filepath = "/mnt/c/Users/wlcoo/Programs/21s-srch-eng-team-landin/cs2341_data/";
    //Use private vector files
    DIR *dir;
    struct dirent *entry;
    dir = opendir(filepath); //trying to access file folder

    if(!dir){
        std::cout<<"directory not found"<<"\n";
        return;
    }else {
        while ((entry = readdir(dir)) != nullptr) {//This while conditional is something I found from stackoverflow
            if (strstr(entry->d_name, ".json")) { //Checks if .json is found in the filename.
                //std::cout<<" \n"<<entry->d_name << std::endl; //If the file is a json print its name....
                files.emplace_back(entry->d_name); //Add the file name to a vector of files
                //
            }
        }
        closedir(dir); //close directory
    }
}//end of openFile()

void DocumentParser::readFile() {
    //loop through the file vector that was passed in
    int totalwordCount;
    handler.treeCounter = 0;
    for(auto & i : files){
        std::string newfilepath = filepath; //this string will contain the absolute path of the directory, you need to make sure a / is at the end of your command line argument
        newfilepath+=i; //This appends the filename from the filename vector onto the absolute path, essentially making a new file directory
        std::ifstream input(newfilepath);
        json j = json::object(); //Make a json object
        input>> j; //read in the json file
        input.close();
        std::string fileID = j["paper_id"]; //ID Gather Successful !!!!
        //std::cout<<"The file ID is: "<<fileID<<"\n";
        std::string fileTitle = j["metadata"]["title"]; //Title Gather Successful !!!!

        int counter =0;
        std::string fileAbstract; //Abstract Successful !!!
        int abstractSize = j["abstract"].size();
        for(int k =0; k<abstractSize; k++){
            fileAbstract+= j["abstract"][k]["text"];
            fileAbstract+= " ";
        }
        counter =0;
        std::string fileBodyText; //Body Texts Successful !!!!
        int bodyTextSize = j["body_text"].size();
        for(int l=0; l<bodyTextSize; l++){
            fileBodyText+= j["body_text"][l]["text"];
            fileBodyText+= " ";
        }
        std::string fileAuthors;
        int allAuthors = j["metadata"]["authors"].size();
        for(int m=0; m<allAuthors; m++){
            fileAuthors+= j["metadata"]["authors"][m]["last"];
            fileAuthors+= ", ";
        }

        std::string entireFile;
        entireFile+=fileTitle;
        entireFile+=fileAbstract;
        entireFile+=fileBodyText;
        Article doc(fileID,fileTitle,fileAbstract,fileBodyText,fileAuthors);
        articles.push_back(doc);
//        entireFile.append(fileTitle);
//        entireFile.append(fileAbstract);
//        entireFile.append(fileBodyText);//Now the Title, abstract, body text
        //std::cout<<"Entire File text of "<<v[i]<<": "<<entireFile<<"\n\n\n";
        //std::vector<std::string> words;
        //Transform all authors to lowercase
        transform(fileAuthors.begin(),fileAuthors.end(),fileAuthors.begin(),::tolower);
        std::stringstream aa(fileAuthors);
        handler.wordCounter=0;
        while(aa>>fileAuthors){
            fileAuthors=handler.remPunc(fileAuthors);
            authorHash.insert(fileAuthors,fileID);
        }
        std::istringstream ss(entireFile);
        while(ss>>entireFile){
            //std::cout<<entireFile<<"\n";
            // checkWords
            //words.push_back(entireFile);
            totalwordCount++;
            handler.checkToAddWord(entireFile, fileID);
        }
    }
    std::cout<<"finished reading"<<"\nThere are "<<handler.treeCounter<< " unique items in the tree\n";
    std::cout<<"There were "<<articles.size()<< " complete files inside the dataset\n";
    std::cout<<"There were about "<<handler.wordCounter<<" words added to the tree in each file\n";
    //std::cout<<"The total amount of words were: "<<totalwordCount<<"\n";

    //This is the original code for the speed demo

    //processQuery(searchWord);
//    searchWord = handler.remPunc(searchWord);
//    transform(searchWord.begin(),searchWord.end(),searchWord.begin(),::tolower);
//    //std::cout<<searchWord;
//    Word checker(searchWord,emptyVector);
//    if(handler.allindex.isFound(checker)){
//        processQuery(searchWord);
//    }else{
//        std::cout<<"\""<<searchWord<<"\""<<" not found in directory";
//    }

    //std::cout<<"The amount of words in vector: "<<handler.allindexVector.size()<<"\n";
//    handler.printExample(handler.allindexVector);

}

void DocumentParser::process1Query(std::vector<std::string> &v) { //for only 1 search word
    bool authorFlag,notFlag, authorNotFound, notNotFound;
    authorFlag = notFlag = authorNotFound = notNotFound = false;
    std::vector<Article> queryArticles, orderedQueryArticles;
    std::vector<Word> wordsinQuery;
    std::vector<std::string> everyID, authorIds, notwordIds, nonKeyWordsearchWords;
    std::vector<int> wordFrequencies;
    for(int i=0; i<v.size(); i++){
        if(v[i]=="AUTHOR"){ //If AUTHOR is in the search, check if the next word is an author in the author hash table
            v[i+1] = handler.remPunc(v[i+1]);
            transform(v[i+1].begin(),v[i+1].end(),v[i+1].begin(),::tolower);
            Porter2Stemmer::stem(v[i+1]);
            if(authorHash.isFound(v[i+1])){ //If the author that was inputed is found, return all the ids that author has written
                authorIds = authorHash.getValueFromKey(v[i+1]);
                authorFlag=true;
                std::cout<<"Author found, this author has written "<<authorIds.size()<<" articles in this dataset\n";
            }else{
                std::cout<<"Author not found\n";
                authorNotFound =true;
            }
            if(i+2<v.size() && authorNotFound== false){ //Check if a NOT was entered after AUTHOR
                if(v[i+2]=="NOT"){
                    v[i+3] = handler.remPunc(v[i+3]);
                    transform(v[i+3].begin(),v[i+3].end(),v[i+3].begin(),::tolower);
                    Porter2Stemmer::stem(v[i+3]);
                    Word checker(v[i+3],emptyVector);
                    if(handler.allindex.isFound(checker)){
                        Word searchedWord = handler.allindex.searchT(v[i+1]);
                        notwordIds = searchedWord.getIDs(); //get the IDs of the found word and put it into the notwordIds vector
                        notFlag = true;
                    }else{
                        std::cout<<"\""<<v[i+3]<<"\""<<" not found in directory\n";
                    }
                    std::cout<<"NOT after AUTHOR recognized\n";
                }
            }
            break;
        }else if(v[i]=="NOT"){ //If NOT is in search, check if the next word is in the Tree
            v[i+1] = handler.remPunc(v[i+1]);
            transform(v[i+1].begin(),v[i+1].end(),v[i+1].begin(),::tolower);
            Porter2Stemmer::stem(v[i+1]);
            Word checker(v[i+1],emptyVector);
            if(handler.allindex.isFound(checker)){
                Word searchedWord = handler.allindex.searchT(v[i+1]);

                notwordIds = searchedWord.getIDs(); //get the IDs of the found word and put it into the notwordIds vector
                notFlag = true;
            }else{
                std::cout<<"\""<<v[i+1]<<"\""<<" not found in directory\n";
                notNotFound = true;
            }
            std::cout<<"NOT recognized\n";
            break;
        }else{
            v[i] = handler.remPunc(v[i]);
            transform(v[i].begin(),v[i].end(),v[i].begin(),::tolower);
            Porter2Stemmer::stem(v[i]);
            Word checker(v[i],emptyVector);
            if(handler.allindex.isFound(checker)){ //Find if the searched word is in the AVL tree, if it is, add it to the wordsinQueryVector
                Word searchedWord = handler.allindex.searchT(v[i]);
                wordsinQuery.push_back(searchedWord);
                nonKeyWordsearchWords.push_back(v[i]);
                //std::cout<<"Found "<<searchedWord.getIDs().size()<<" times\n";
                resultFound = true;
            }else{
                std::cout<<"\""<<v[i]<<"\""<<" not found in directory\n";
            }
        }
    }

    std::cout<<"One moment please...\n";
    //This loop contains all the ids of the searched results
    for(int i=0; i<wordsinQuery.size(); i++){ //Push the ids of all the words in the wordsInQuery vector to the everyId vector which hold every single Id of all words in query
        std::vector<std::string> tempId = wordsinQuery[i].getIDs();
        for(int j=0; j<tempId.size(); j++){
            everyID.push_back(tempId[j]);
        }
    }
    //sets can only have unique values, i'm using this to delete duplicate IDs from everyId vector
    std::set<std::string> idSet(everyID.begin(), everyID.end());
    //I am going to transform this back into a vector for ease of use
    std::vector<std::string> combinedIds(idSet.begin(), idSet.end()); //This will be the vector used to compare against the AUTHOR and NOT flags

    if(authorFlag){ //If author was inside the query...
        std::vector<std::string> idWithAuthors;
        for(int i=0; i<combinedIds.size();i++){
            for(int j=0; j<authorIds.size(); j++){
                if(combinedIds[i]==authorIds[j]){ //The combined IDs vector only has the IDs of the search word written by the searched author
                    idWithAuthors.push_back(authorIds[j]);
                }
            }
        }
        combinedIds = idWithAuthors; //the combinedIds vector will now be the IDs that contain the search query written by the AUTHOR searched
    }//end of author flag

    if(notFlag){//If NOT was inside the query....
        std::vector<std::string> idWithNot;
        for(int i=0; i<notwordIds.size(); i++){
            for(int j=0; j<combinedIds.size();j++){
                if(notwordIds[i]==combinedIds[j]){
                    idWithNot.push_back(combinedIds[j]);
                }
            }
        }
        std::cout<<"the amount of ids of the NOT word inside the searched words: "<<idWithNot.size()<<"\n";
        std::sort(combinedIds.begin(), combinedIds.end());
        std::sort(idWithNot.begin(), idWithNot.end());

        //This set symmetric difference was code that I found on stack overflow, it pushes, unique items from 2 vectors into a new vector
        std::vector<std::string> combinedWithNot;
        std::set_symmetric_difference(combinedIds.begin(),combinedIds.end(),
                                      idWithNot.begin(),idWithNot.end(),
                                      std::back_inserter(combinedWithNot));

        std::cout<<"the amount of ids in the search after deleting the NOT word:"<<combinedWithNot.size()<<"\n";
        combinedIds = combinedWithNot;//The combinedIds vector will now be the Ids that contain the search word, and also do not contain the NOT search word
    }//end of NOT flag
    if(authorNotFound){
        std::cout<<"No authors were found please search with a different author\n";
        searchVector.clear();
        return;
    }
    if(notNotFound){
        std::cout<<"No not words found please search with a different NOT\n";
        searchVector.clear();
        return;
    }

    std::set<std::string> checker(combinedIds.begin(),combinedIds.end());
    std::vector<std::string> checkedIds(checker.begin(), checker.end()); //double check for duplicates.

    for(int i=0; i<checkedIds.size(); i++){ //Finds the associated articles from the doubled checked ID vector
        Article temp = findArticleFromID(checkedIds[i]);
        queryArticles.push_back(temp);
    }

    for(int i=0; i<queryArticles.size(); i++){ //iterate through found articles, search each article for the searched words. Count word frequency of searched word.
        int wordFreq =0;
        int totalWordFreq =0; //this is total wordfreq
        for(int j=0; j<nonKeyWordsearchWords.size(); j++){
            wordFreq =0;
            std::string totalText;
            //DOcument wordFrequency
            totalText+=queryArticles[i].getArticleTitle();
            totalText+= " ";
            totalText+=queryArticles[i].getArticleAbstract();
            totalText+= " ";
            totalText+=queryArticles[i].getArticleBodyText();
            std::istringstream ss(totalText);
            while(ss>>totalText){
                totalText= handler.remPunc(totalText);
                transform(totalText.begin(),totalText.end(),totalText.begin(),::tolower);
                Porter2Stemmer::stem(totalText);
                if(totalText==nonKeyWordsearchWords[j]){ //iterates through each word of Article, if searched word equals the word, increase counter
                    wordFreq++;
                }
            }
            totalWordFreq+=wordFreq;
        }
        queryArticles[i].setArticleWordFrequency(totalWordFreq);
        wordFrequencies.push_back(totalWordFreq);
    }
    std::cout<<"The amount of articles found: "<<queryArticles.size()<<"\n";
    //found this sort on stackoverflow, it sorts a vector based on int values
    sort(wordFrequencies.begin(),wordFrequencies.end(),greater<int>());
    for(int i=0; i<wordFrequencies.size(); i++){
        for(int j=0; j<queryArticles.size(); j++){
            if(wordFrequencies[i]==queryArticles[j].getArticleWordFrequency()){
                orderedQueryArticles.push_back(queryArticles[j]);
            }
        }
    }
    std::cout<<"\nThe 15 most relevant articles to your search are:\n";
    for(int i=0; i<orderedQueryArticles.size(); i++){
        if(i>14){
            break;
        }
        std::cout<<orderedQueryArticles[i].getArticleTitle()<<"\nMade by:"<<orderedQueryArticles[i].getArticlesAuthors()<<"\n";
        std::cout<<orderedQueryArticles[i].getArticleID()<<"\nWith word count:"<<orderedQueryArticles[i].getArticleWordFrequency()<<"\n";
        std::cout<<"To see the first 300 words of this article enter: "<<i+1<<"\n\n";
    }
    int userInput;
    std::cout<<"Would you like to see more from an article? Enter it's number. If not, please enter \"-1\" ";
    std::cin>>userInput;
    for(int i=0; i<15; i++){
        if(userInput== -1){
            break;
        }else if(userInput==i+1){
            std::cout<<"You wanted to see more of "<< orderedQueryArticles[i].getArticleTitle()<<"\n";
            printFirst300Words(orderedQueryArticles[i]);
            std::cout<<"... \n";
        }
    }
}

void DocumentParser::processOrQuery(std::vector<std::string> &v){ //If OR is the first word, for an OR query
    bool authorFlag,notFlag, authorNotFound, notNotFound;
    authorFlag = notFlag = authorNotFound = notNotFound = false;
    std::vector<Article> queryArticles, orderedQueryArticles;
    std::vector<Word> wordsinQuery;
    std::vector<std::string> everyID, authorIds, notwordIds, nonKeyWordsearchWords;
    std::vector<int> wordFrequencies;
    for(int i=1; i<v.size(); i++){
        if(v[i]=="AUTHOR"){ //If AUTHOR is in the search, check if the next word is an author in the author hash table
            v[i+1] = handler.remPunc(v[i+1]);
            transform(v[i+1].begin(),v[i+1].end(),v[i+1].begin(),::tolower);
            Porter2Stemmer::stem(v[i+1]);
            if(authorHash.isFound(v[i+1])){ //If the author that was inputed is found, return all the ids that author has written
                authorIds = authorHash.getValueFromKey(v[i+1]);
                authorFlag=true;
                std::cout<<"Author found, this author has written "<<authorIds.size()<<" articles in this dataset\n";
            }else{
                std::cout<<"Author not found\n";
                authorNotFound=true;
            }
            if(i+2<v.size() && authorNotFound==false){ //Check if a NOT was entered after AUTHOR
                if(v[i+2]=="NOT"){
                    v[i+3] = handler.remPunc(v[i+3]);
                    transform(v[i+3].begin(),v[i+3].end(),v[i+3].begin(),::tolower);
                    Porter2Stemmer::stem(v[i+3]);
                    Word checker(v[i+3],emptyVector);
                    if(handler.allindex.isFound(checker)){
                        Word searchedWord = handler.allindex.searchT(v[i+1]);
                        notwordIds = searchedWord.getIDs(); //get the IDs of the found word and put it into the notwordIds vector
                        //std::cout<<"Found "<<searchedWord.getIDs().size()<<" times\n";
                        notFlag = true;
                    }else{
                        std::cout<<"\""<<v[i+3]<<"\""<<" not found in directory\n";
                    }
                    std::cout<<"NOT after AUTHOR recognized\n";
                }
            }
            break;
        }else if(v[i]=="NOT"){ //If NOT is in search, check if the next word is in the Tree
            v[i+1] = handler.remPunc(v[i+1]);
            transform(v[i+1].begin(),v[i+1].end(),v[i+1].begin(),::tolower);
            Porter2Stemmer::stem(v[i+1]);
            Word checker(v[i+1],emptyVector);
            if(handler.allindex.isFound(checker)){
                Word searchedWord = handler.allindex.searchT(v[i+1]);
//                wordsinQuery.push_back(searchedWord);
//                nonKeyWordsearchWords.push_back(v[i+1]);
                notwordIds = searchedWord.getIDs(); //get the IDs of the found word and put it into the notwordIds vector
                //std::cout<<"Found "<<searchedWord.getIDs().size()<<" times\n";
                notFlag = true;
            }else{
                std::cout<<"\""<<v[i+1]<<"\""<<" not found in directory\n";
                notNotFound=true;
            }
            std::cout<<"NOT recognized\n";
            break;
        }else{
            v[i] = handler.remPunc(v[i]);
            transform(v[i].begin(),v[i].end(),v[i].begin(),::tolower);
            Porter2Stemmer::stem(v[i]);
            //std::cout<<v[i]<<"!!!\n";
            Word checker(v[i],emptyVector);
            if(handler.allindex.isFound(checker)){ //Find if the searched word is in the AVL tree, if it is, add it to the wordsinQueryVector
                Word searchedWord = handler.allindex.searchT(v[i]);
                wordsinQuery.push_back(searchedWord);
                nonKeyWordsearchWords.push_back(v[i]);
                //std::cout<<"Found "<<searchedWord.getIDs().size()<<" times\n";
                resultFound = true;
            }else{
                std::cout<<"\""<<v[i]<<"\""<<" not found in directory\n";
            }
        }

    }
    std::cout<<"One moment please...\n";
    //This loop contains all the ids of the searched results
    for(int i=0; i<wordsinQuery.size(); i++){ //Push the ids of all the words in the wordsInQuery vector to the everyId vector which hold every single Id of all words in query
        std::vector<std::string> tempId = wordsinQuery[i].getIDs();
        for(int j=0; j<tempId.size(); j++){
            everyID.push_back(tempId[j]);
        }
    }
    //sets can only have unique values, i'm using this to delete duplicate IDs from everyId vector
    std::set<std::string> idSet(everyID.begin(), everyID.end());
    //I am going to transform this back into a vector for ease of use
    std::vector<std::string> combinedIds(idSet.begin(), idSet.end()); //This will be the vector used to compare against the AUTHOR and NOT flags

    if(authorFlag){ //If author was inside the query...
        std::vector<std::string> idWithAuthors;
        for(int i=0; i<combinedIds.size();i++){
            for(int j=0; j<authorIds.size(); j++){
                if(combinedIds[i]==authorIds[j]){ //The combined IDs vector only has the IDs of the search word written by the searched author
                    idWithAuthors.push_back(authorIds[j]);
                }
            }
        }
        combinedIds = idWithAuthors; //the combinedIds vector will now be the IDs that contain the search query written by the AUTHOR searched
    }
    if(notFlag){//If NOT was inside the query....
        std::vector<std::string> idWithNot;
        for(int i=0; i<notwordIds.size(); i++){
            for(int j=0; j<combinedIds.size();j++){
                if(notwordIds[i]==combinedIds[j]){
                    idWithNot.push_back(combinedIds[j]);
                }
            }
        }
        std::cout<<"the amount of ids of the NOT word inside the searched words: "<<idWithNot.size()<<"\n";

        std::sort(combinedIds.begin(), combinedIds.end());
        std::sort(idWithNot.begin(), idWithNot.end());

        //This set symmetric difference was code that I found on stack overflow, it pushes, unique items from 2 vectors into a new vector
        std::vector<std::string> combinedWithNot;
        std::set_symmetric_difference(combinedIds.begin(),combinedIds.end(),
                                      idWithNot.begin(),idWithNot.end(),
                                      std::back_inserter(combinedWithNot));

        std::cout<<"the amount of ids in the search after deleting the NOT word:"<<combinedWithNot.size()<<"\n";
        combinedIds = combinedWithNot;//The combinedIds vector will now be the Ids that contain the search word, and also do not contain the NOT search word
    }
    if(authorNotFound){
        std::cout<<"No authors were found please search with a different author\n";
        searchVector.clear();
        return;
    }
    if(notNotFound){
        std::cout<<"No not words found please search with a different NOT\n";
        searchVector.clear();
        return;
    }
    std::set<std::string> checker(combinedIds.begin(),combinedIds.end());
    std::vector<std::string> checkedIds(checker.begin(), checker.end()); //double check for duplicates.

    for(int i=0; i<checkedIds.size(); i++){ //Finds the associated articles from the doubled checked ID vector
        Article temp = findArticleFromID(checkedIds[i]);
        queryArticles.push_back(temp);
        //std::cout<<combinedIds[i]<<"\n";
    }
    for(int i=0; i<queryArticles.size(); i++){ //iterate through found articles, search each article for the searched words. Count word frequency of searched word.
        int wordFreq =0;
        int totalWordFreq =0; //this is total wordfreq
        for(int j=0; j<nonKeyWordsearchWords.size(); j++){
            wordFreq =0;
            std::string totalText;
            //DOcument wordFrequency
            totalText+=queryArticles[i].getArticleTitle();
            totalText+= " ";
            totalText+=queryArticles[i].getArticleAbstract();
            totalText+= " ";
            totalText+=queryArticles[i].getArticleBodyText();
            std::istringstream ss(totalText);
            while(ss>>totalText){
                totalText= handler.remPunc(totalText);
                transform(totalText.begin(),totalText.end(),totalText.begin(),::tolower);
                Porter2Stemmer::stem(totalText);
                if(totalText==nonKeyWordsearchWords[j]){ //iterates through each word of Article, if searched word equals the word, increase counter
                    wordFreq++;
                }
            }
            totalWordFreq+=wordFreq;
        }
        queryArticles[i].setArticleWordFrequency(totalWordFreq);
        wordFrequencies.push_back(totalWordFreq);
    }
    //std::cout<<"The amount of ids in both: "<<checkedIds.size()<<"\n";
    std::cout<<"The amount of articles found: "<<queryArticles.size()<<"\n";

//  std::cout<<"The non keyword search words were: ";
//    for(int i=0; i<nonKeyWordsearchWords.size(); i++){
//        std::cout<<nonKeyWordsearchWords[i]<<" ";
//        //parseFoundArticles(queryArticles,nonKeyWordsearchWords[i]);
//    }
//    std::cout<<"\n";
//    std::cout<<"Word frequency of found articles: \n";
//    for(int i=0; i<queryArticles.size(); i++){
//        //std::cout<<"\t"<<queryArticles[i].getArticleWordFrequency()<<"\n";
//        std::cout<<queryArticles[i].getArticleID()<<" with freq:"<< queryArticles[i].getArticleWordFrequency()<<"\n";
//    }
//    std::cout<<"The unordered word frequencies of the articles were:\n";
//    for(int i=0; i<wordFrequencies.size(); i++){
//        std::cout<<wordFrequencies[i]<<"\n";
//    }

    //found this sort on stackoverflow, it sorts a vector based on int values
    sort(wordFrequencies.begin(),wordFrequencies.end(),greater<int>());
    for(int i=0; i<wordFrequencies.size(); i++){
        for(int j=0; j<queryArticles.size(); j++){
            if(wordFrequencies[i]==queryArticles[j].getArticleWordFrequency()){
                orderedQueryArticles.push_back(queryArticles[j]);
            }
        }
    }
    std::cout<<"\nThe 15 most relevant articles to your search are:\n";
    for(int i=0; i<orderedQueryArticles.size(); i++){
        if(i>14){
            break;
        }
        std::cout<<orderedQueryArticles[i].getArticleTitle()<<"\nMade by:"<<orderedQueryArticles[i].getArticlesAuthors()<<"\n";
        std::cout<<orderedQueryArticles[i].getArticleID()<<"\nWith word count:"<<orderedQueryArticles[i].getArticleWordFrequency()<<"\n";
        std::cout<<"To see the first 300 words of this article enter: "<<i+1<<"\n\n";
    }
    int userInput;
    std::cout<<"Would you like to see more from an article? Enter it's number. If not, please enter \"-1\" ";
    std::cin>>userInput;
    for(int i=0; i<15; i++){
        if(userInput== -1){
            break;
        }else if(userInput==i+1){
            std::cout<<"You wanted to see more of "<< orderedQueryArticles[i].getArticleTitle()<<"\n";
            printFirst300Words(orderedQueryArticles[i]);
            std::cout<<"... \n";
        }
    }
    //All Articles in queryArticles now have a wordFrequency of the searched word/words

}

void DocumentParser::processANDQuery(std::vector<std::string> &v){ //For an AND query.
    bool authorFlag,notFlag, authorNotFound, notNotFound;
    authorFlag = notFlag = authorNotFound = notNotFound = false;
    std::vector<Article> queryArticles, orderedQueryArticles;
    std::vector<Word> wordsinQuery;
    std::vector<std::string> everyID, authorIds, notwordIds, nonKeyWordsearchWords;
    std::vector<int> wordFrequencies;
    for(int i=1; i<v.size(); i++){
        if(v[i]=="AUTHOR"){ //If AUTHOR is in the search, check if the next word is an author in the author hash table
            v[i+1] = handler.remPunc(v[i+1]);
            transform(v[i+1].begin(),v[i+1].end(),v[i+1].begin(),::tolower);
            Porter2Stemmer::stem(v[i+1]);
            if(authorHash.isFound(v[i+1])){ //If the author that was inputed is found, return all the ids that author has written
                authorIds = authorHash.getValueFromKey(v[i+1]);
                authorFlag=true;
                std::cout<<"Author found, this author has written "<<authorIds.size()<<" articles in this dataset\n";
            }else{
                std::cout<<"Author not found\n";
                authorNotFound =true;

            }
            if(i+2<v.size()&& authorNotFound== false){ //Check if a NOT was entered after AUTHOR
                if(v[i+2]=="NOT"){
                    v[i+3] = handler.remPunc(v[i+3]);
                    transform(v[i+3].begin(),v[i+3].end(),v[i+3].begin(),::tolower);
                    Porter2Stemmer::stem(v[i+3]);
                    Word checker(v[i+3],emptyVector);
                    if(handler.allindex.isFound(checker)){
                        Word searchedWord = handler.allindex.searchT(v[i+1]);
                        notwordIds = searchedWord.getIDs(); //get the IDs of the found word and put it into the notwordIds vector
                        //std::cout<<"Found "<<searchedWord.getIDs().size()<<" times\n";
                        notFlag = true;
                    }else{
                        std::cout<<"\""<<v[i+3]<<"\""<<" not found in directory\n";
                    }
                    std::cout<<"NOT after AUTHOR recognized\n";
                    }
                }
            break;
        }else if(v[i]=="NOT"){ //If NOT is in search, check if the next word is in the Tree
            v[i+1] = handler.remPunc(v[i+1]);
            transform(v[i+1].begin(),v[i+1].end(),v[i+1].begin(),::tolower);
            Porter2Stemmer::stem(v[i+1]);
            Word checker(v[i+1],emptyVector);
            if(handler.allindex.isFound(checker)){
                Word searchedWord = handler.allindex.searchT(v[i+1]);
//
                notwordIds = searchedWord.getIDs(); //get the IDs of the found word and put it into the notwordIds vector
                //std::cout<<"Found "<<searchedWord.getIDs().size()<<" times\n";
                notFlag = true;
            }else{
                std::cout<<"\""<<v[i+1]<<"\""<<" not found in directory\n";
                notNotFound = true;
            }
            std::cout<<"NOT recognized\n";
            break;
        }else{
            v[i] = handler.remPunc(v[i]);
            transform(v[i].begin(),v[i].end(),v[i].begin(),::tolower);
            Porter2Stemmer::stem(v[i]);
            //std::cout<<v[i]<<"!!!\n";
            Word checker(v[i],emptyVector);
            if(handler.allindex.isFound(checker)){ //Find if the searched word is in the AVL tree, if it is, add it to the wordsinQueryVector
                Word searchedWord = handler.allindex.searchT(v[i]);
                wordsinQuery.push_back(searchedWord);
                nonKeyWordsearchWords.push_back(v[i]);
                //std::cout<<"Found "<<searchedWord.getIDs().size()<<" times\n";
                resultFound = true;
            }else{
                std::cout<<"\""<<v[i]<<"\""<<" not found in directory\n";
            }
        }


    }
    std::cout<<"One moment please...\n";

    //Since this is an AND statement, the Ids of the first word, must be equal to the ids of the remaining search words
    std::vector<std::string> firstSearchIds = wordsinQuery[0].getIDs();
    std::cout<<"the size of the starting search word"<<firstSearchIds.size()<<"\n";
    for(int i=1; i<wordsinQuery.size(); i++){ //Iterate through the wordsInQueryVector to create a vector equal to its Ids
        std::vector<std::string> andCheckerIds = wordsinQuery[i].getIDs();
        std::cout<<"the size of the "<<i+1<<"word "<<andCheckerIds.size()<<"\n";
        for(int j=0; j<firstSearchIds.size(); j++){
            for(int k=0; k<andCheckerIds.size(); k++){
                if(firstSearchIds[j]==andCheckerIds[k]){
                    everyID.push_back(andCheckerIds[k]);
                }
            }
        }
    }
    std::cout<<"Amount of total AND ids are:"<<everyID.size()<<"\n";
//    std::cout<<"Lets test the AND merging\n";
//    for(int i=0; i<everyID.size(); i++){
//        std::cout<<everyID[i]<<" ";
//    }
//    std::cout<<" ";
    //sets can only have unique values, i'm using this to delete duplicate IDs from everyId vector
    std::set<std::string> idSet(everyID.begin(), everyID.end());
    //I am going to transform this back into a vector for ease of use
    std::vector<std::string> combinedIds(idSet.begin(), idSet.end()); //This will be the vector used to compare against the AUTHOR and NOT flags
    std::cout<<"Amount of total AND ids after transformed from a set and back: "<<combinedIds.size()<<"\n";

    if(authorFlag){ //If author was inside the query...
        std::vector<std::string> idWithAuthors;
        for(int i=0; i<combinedIds.size();i++){
            for(int j=0; j<authorIds.size(); j++){
                if(combinedIds[i]==authorIds[j]){ //The combined IDs vector only has the IDs of the search word written by the searched author
                    idWithAuthors.push_back(authorIds[j]);
                }
            }
        }
        combinedIds = idWithAuthors; //the combinedIds vector will now be the IDs that contain the search query written by the AUTHOR searched
    }
    if(notFlag){//If NOT was inside the query....
        std::vector<std::string> idWithNot;
        for(int i=0; i<notwordIds.size(); i++){
            for(int j=0; j<combinedIds.size();j++){
                if(notwordIds[i]==combinedIds[j]){
                    idWithNot.push_back(combinedIds[j]);
                }
            }
        }
//        std::cout<<"Combined Ids vector\n";
//        for(int i=0; i<combinedIds.size();i++){
//            std::cout<<combinedIds[i]<<" \n";
//        }
        std::cout<<"the amount of ids of the NOT word inside the searched words: "<<idWithNot.size()<<"\n";

        std::sort(combinedIds.begin(), combinedIds.end());
        std::sort(idWithNot.begin(), idWithNot.end());

        //This set symmetric difference was code that I found on stack overflow, it pushes, unique items from 2 vectors into a new vector
        std::vector<std::string> combinedWithNot;
        std::set_symmetric_difference(combinedIds.begin(),combinedIds.end(),
                                      idWithNot.begin(),idWithNot.end(),
                                      std::back_inserter(combinedWithNot));

//        std::cout<<"set symmetric difference check: plz work\n";
//        for(int i=0; i<combinedWithNot.size(); i++){
//            std::cout<<combinedWithNot[i]<<" ";
//        }

        std::cout<<"the amount of ids in the search after deleting the NOT word:"<<combinedWithNot.size()<<"\n";
        combinedIds = combinedWithNot;//The combinedIds vector will now be the Ids that contain the search word, and also do not contain the NOT search word
    }
    if(authorNotFound){
        std::cout<<"No authors were found please search with a different author\n";
        searchVector.clear();
        return;
    }
    if(notNotFound){
        std::cout<<"No not words found please search with a different NOT\n";
        searchVector.clear();
        return;
    }
    std::set<std::string> checker(combinedIds.begin(),combinedIds.end());
    std::vector<std::string> checkedIds(checker.begin(), checker.end()); //double check for duplicates.
    //std::cout<<"the amount of ids after NOT flag:"<<checkedIds.size()<<"\n";

    for(int i=0; i<checkedIds.size(); i++){ //Finds the associated articles from the doubled checked ID vector
        Article temp = findArticleFromID(checkedIds[i]);
        queryArticles.push_back(temp);
        //std::cout<<combinedIds[i]<<"\n";
    }
    std::cout<<"non keyword search words: \n";
    for(int i=0; i<nonKeyWordsearchWords.size(); i++){
        std::cout<<nonKeyWordsearchWords[i]+" ";
    }
    std::cout<<"\n";
    for(int i=0; i<queryArticles.size(); i++){ //iterate through found articles, search each article for the searched words. Count word frequency of searched word.
        int wordFreq =0;
        int totalWordFreq =0; //this is total wordfreq
        for(int j=0; j<nonKeyWordsearchWords.size(); j++){
            wordFreq =0;
            std::string totalText;
            totalText+=queryArticles[i].getArticleTitle();
            totalText+= " ";
            totalText+=queryArticles[i].getArticleAbstract();
            totalText+= " ";
            totalText+=queryArticles[i].getArticleBodyText();
            std::istringstream ss(totalText);
            while(ss>>totalText){
                totalText= handler.remPunc(totalText);
                transform(totalText.begin(),totalText.end(),totalText.begin(),::tolower);
                Porter2Stemmer::stem(totalText);
                if(totalText==nonKeyWordsearchWords[j]){ //iterates through each word of Article, if searched word equals the word, increase counter
                    wordFreq++;
                }
            }
            totalWordFreq+=wordFreq;
        }
        queryArticles[i].setArticleWordFrequency(totalWordFreq);
        wordFrequencies.push_back(totalWordFreq);
    }
    std::cout<<"The amount of articles found: "<<queryArticles.size()<<"\n";

    //found this sort on stackoverflow, it sorts a vector based on int values
    sort(wordFrequencies.begin(),wordFrequencies.end(),greater<int>());
    for(int i=0; i<wordFrequencies.size(); i++){
        for(int j=0; j<queryArticles.size(); j++){
            if(wordFrequencies[i]==queryArticles[j].getArticleWordFrequency()){
                orderedQueryArticles.push_back(queryArticles[j]);
            }
        }
    }
    std::cout<<"\nThe 15 most relevant articles to your search are:\n";
    for(int i=0; i<orderedQueryArticles.size(); i++){
        if(i>14){
            break;
        }
        std::cout<<orderedQueryArticles[i].getArticleTitle()<<"\nMade by:"<<orderedQueryArticles[i].getArticlesAuthors()<<"\n";
        std::cout<<orderedQueryArticles[i].getArticleID()<<"\nWith word count:"<<orderedQueryArticles[i].getArticleWordFrequency()<<"\n";
        std::cout<<"To see the first 300 words of this article enter: "<<i+1<<"\n\n";
    }
    int userInput;
    std::cout<<"Would you like to see more from an article? Enter it's number. If not, please enter \"-1\" ";
    std::cin>>userInput;
    for(int i=0; i<15; i++){
        if(userInput== -1){
            break;
        }else if(userInput==i+1){
            std::cout<<"You wanted to see more of "<< orderedQueryArticles[i].getArticleTitle()<<"\n";
            printFirst300Words(orderedQueryArticles[i]);
            std::cout<<"... \n";
        }
    }
    //All Articles in queryArticles now have a wordFrequency of the searched word/words

}

void DocumentParser::processQuery(std::string &search) { //original process query for speed demo
    std::vector<Word> wordsinQuery;
    search = handler.remPunc(search);
    transform(search.begin(),search.end(),search.begin(),::tolower);
    Porter2Stemmer::stem(search);
    Word checker(search,emptyVector);
    if(handler.allindex.isFound(checker)){
        Word searchedWord = handler.allindex.searchT(search);
        wordsinQuery.push_back(searchedWord);
        std::cout<<"Found "<<searchedWord.getIDs().size()<<" times\n";
        resultFound = true;
    }else{
        std::cout<<"\""<<search<<"\""<<" not found in directory\n";
    }

}
std::vector<std::string> DocumentParser::createQueryVector(std::string &s) { //used in original plan but scrapped
    std::vector<std::string> searchVector;
    std::istringstream ss(s);
    while(ss>>s){
        searchVector.push_back(s);
    }
    return std::vector<std::string>();
}
void DocumentParser::displayOrResults(std::vector<std::string>&v2) { //for testing purposes
    for (int i = 0; i < searchResuts.size(); i++) {
        for (int j = 0; j < v2.size(); j++) {
            if (searchResuts[i][j] == v2[j]) {
                std::cout << searchResuts[i][j] << "\n";
            }
        }
    }
}

void DocumentParser::readStopWords() { //used to enter stopwords into a map
    std::string stopword;
    std::ifstream input("../Stopwords.txt");
    input>>stopword;
    stopwords.insert(std::pair<std::string,std::string>(stopword,stopword));
    while(!input.eof()){
        input>>stopword;
        stopwords.insert(std::pair<std::string,std::string>(stopword,stopword));
    }
//    for (auto const &pair: stopwords) { //for display purposes
//        std::cout << "{" << pair.first << ": " << pair.second << "}\n";
//    }
}

Word DocumentParser::getWordFromString(std::string &s) {
    return handler.allindex.searchT(s);
}

Article DocumentParser::findArticleFromID(std::string &s) { //essential part of search algorithm, transforms an ID back into an Article Obj
    for(int i=0; i<articles.size(); i++){
        if(s == articles[i].getArticleID()){
            return articles[i];
        }
    }
}

void DocumentParser::parseFoundArticles(std::vector<Article> &articles, std::string&search) {

}

void DocumentParser::printFirst300Words(Article &article) { //for displaying 300 of the words of the article
    int counter =0;
    std::string body = article.getArticleBodyText();
    std::istringstream ss(body);
    while(ss>>body){
        if(counter<300){
            std::cout<< body<<" ";
        }
        counter++;
    }

    //article.setArticleBodyText();
}





