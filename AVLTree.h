
#ifndef SEARCH_ENGINE_AVLTREE_H
#define SEARCH_ENGINE_AVLTREE_H
#pragma once
#include <fstream>

template<typename T>
class AVLTree{
private:
    class AVLNode {
    public:
        T data;
        AVLNode *left, *right;
        int height;
        AVLNode(const T &d, AVLNode *lt, AVLNode *rt, int h=0, int s=0):data(d),left(lt),right(rt),height(h) {}
        T& searchTree(std::string&key){
            if(data == key ){
                return data;
            }else if(this->data < key){
                return right->searchTree(key);
            }else if(this->data > key){
                return left->searchTree(key);
            }
        }
        ~AVLNode(){//This was a destructor I found on stack overflow that was necessary for deleting my Tree.
            if(left!= nullptr){
                delete right;
                right= nullptr;
            }
            if(right!= nullptr){
                delete left;
                left= nullptr;
            }
        }
    };
    AVLNode *root;
public:
    AVLTree():root(NULL){}
    AVLTree(const AVLTree&rhs):root(NULL){
        *this=rhs;
    }
    ~AVLTree(){
        makeEmpty();
    }
//
//    AVLNode* insert(T& key, AVLNode*& node){//This does not take duplicates into account
//        if(node == nullptr){
//            return new AVLNode(key,nullptr,nullptr);
//        }else if(node->data<key){
//            node->right= insert(key, node->right);
//        }else if(key<node->data){
//            node ->left = insert(key, node->left);
//        }
//        balance(node, key);
//
//        return node;
//    }

    void newInsert(T &key, AVLNode*& node){ //This one takes duplicates into account, I use this one, inspired by geeksforgeeks
        if(node == nullptr){
            node = new AVLNode(key, nullptr, nullptr);
        }
        else if(key < node->data){
            newInsert(key, node->left);
            if(height(node->left) - height(node->right) >1){
                if(key < node->left->data)
                    rotateWithLeftChild(node);
                else
                    doubleWithLeftChild(node);
            }
        }
        else if(node->data < key){

            newInsert(key, node->right);

            if(height(node->right) - height(node->left) >1){
                if(node->right->data < key)
                    rotateWithRightChild(node);
                else
                    doubleWithRightChild(node);
            }
        }
        else {//I need this for my objects, this + operator will add duplicates together. I overloaded my + operator to call the push_back function of my Word object's vector
            node->data = node->data + key;
        }
        node->height = max(height(node->left), height(node->right)) + 1;
    }

    void insert(T& x){ //calls the insert that takes duplicates into account
        newInsert(x, root);
    }


//    AVLNode* balance(AVLNode*& t, T&key) {
//        if (height(t->left) - height(t->right) > 1) {
//            if (height(t->left->left) >= height(t->left->right)) {
//                rotateWithLeftChild(t);
//            } else {
//                doubleWithLeftChild(t);
//            }
//        } else if (height(t->right) - height(t->left) > 1) {
//            if (height(t->right->left) >= height(t->right->right)) {
//                doubleWithRightChild(t);
//            } else {
//                rotateWithRightChild(t);
//            }
//        }else {
//            t->data = t->data + key;
//        }
////        }else if(height(t->right)==height(t->left)){
////            t->data = t->data + key;
////        }
//            t->height = max(height(t->left), height(t->right)) + 1;
//            return t;
//        }


    int max(const int &lhs, const int &rhs){
        if(lhs>rhs){
            return lhs;
        }else{
            return rhs;
        }
    }

    int height(AVLNode *&node)const{
        if(node == nullptr){
            return 0;
        }
        return node->height;
    }

    void rotateWithLeftChild(AVLNode*&k2){
        AVLNode*k1 =k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max(height(k2->left),height(k2->right))+1;
        k2=k1;
    }

    void doubleWithLeftChild(AVLNode*&k3){
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }

    void rotateWithRightChild(AVLNode*&k1){
        AVLNode*k2 = k1 ->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max(height(k1->right),height(k1->left))+1;
        //k2->height = max(height(k2->right),k2->height)+1;
        k1=k2;
    }
    void doubleWithRightChild(AVLNode*&k3){
        rotateWithLeftChild(k3->right);
        rotateWithRightChild(k3);
    }
    void makeEmpty(){
        if(root!= nullptr){
            delete root;
            root=nullptr;
        }
    }
    void printInorder(struct AVLNode* node){//Found online from geeksforgeeks
        if(node ==nullptr){
            return;
        }
        printInorder(node->left);//does recursion on the left child
        std::cout<<node->data<<" ";
        printInorder(node->right);//does recursion on the right child
    }

    void treedisplay(){//this is for the purposes of testing
        printInorder(root);
    }

    void printToFile(struct AVLNode* node, std::ofstream& output){
        if(node== nullptr){
            return;
        }
        printToFile(node->left,output);
        output<<node->data<<"\n";
        printToFile(node->right,output);
    }

    void indexFilePrint(std::ofstream& output){
        printToFile(root,output);
    }





    T& searchT(std::string& key){ //this is a function i call in the program
        return root->searchTree(key);
    }

    bool isFound(T &key){
        return recursiveIsFound(root, key);
    }

    bool recursiveIsFound(AVLNode*&node,T &key){ //Inspiration from stack overflow,
        if(node== nullptr){
            return false;
        }else if(key == node->data){
            return true;
        }else if(key < node->data){ //Have to check left and right childs
            return recursiveIsFound(node->left, key);
        }else if(key > node->data){
            return recursiveIsFound(node->right, key);
        }
    }


};





#endif //SEARCH_ENGINE_AVLTREE_H
