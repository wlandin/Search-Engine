
#ifndef SEARCH_ENGINE_HASHTABLE_H
#define SEARCH_ENGINE_HASHTABLE_H

#include <vector>
#include <iostream>

//This was derived from Dr. Fontenot's in class demonstration, I changed my Value to be a vector because of the nature of this project. Each author would have multiple ids, so I would push back the id to the same author
template<typename K, typename V>
class HashTable {
private:
    struct Entry{
        K key;
        std::vector<V> value;
        Entry(K k, std::vector<V> v):key(k), value(v){}
    };
    std::vector<Entry>* table = nullptr;
    int numberElems =0;
    int tableSize =0;
public:
    HashTable(){
        tableSize =100;
        table = new std::vector<Entry>[tableSize];
    }

    void insert(const K& key, const V& value){
        if(!isFound(key)){
            //If the key is not in Hash Table...
            std::vector<V> temp;
            temp.push_back(value);
            Entry e(key,temp);
            auto hash_value = std::hash<K>{}(key);
            int loc = hash_value % tableSize;
            table[loc].push_back(e);  //insert it
        }else{
            auto location = std::hash<K>{}(key) % tableSize;
            for(auto &c : table[location]){
                if(c.key == key){
                    c.value.push_back(value);
                    //table[location].push_back(value);
                }
            }
        }
    }

    std::vector<V>& getValueFromKey(K& theKey){
        auto location = std::hash<K>{}(theKey) % tableSize;
        for(auto &c : table[location]){
            if(c.key == theKey){
                return c.value;
            }
        }
    }
    bool isFound(const K& theKey){
        auto location = std::hash<K>{}(theKey) % tableSize;
        for(auto &c : table[location]){
            if(c.key == theKey){
                return true;
            }
        }
        return false;
    }
    void printHash(){
//        for(int i=0; i<table->size();i++){
//            std::cout<<table[i].
//        }
//
//        auto location = std::hash<K>{}(this.) % tableSize;
//        for(auto &c : table[location]){
//            if(c.key == theKey){
//                return true;
//            }
//        }
    }

};


#endif //SEARCH_ENGINE_HASHTABLE_H
