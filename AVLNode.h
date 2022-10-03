
#ifndef SEARCH_ENGINE_AVLNODE_H
#define SEARCH_ENGINE_AVLNODE_H

template<typename T>
class AVLNode {
public:
    AVLNode();
    AVLNode(T& data);
    AVLNode(const AVLNode<*& temp);
    ~AVLNode();
    T data;
    AVLNode<T> *left, *right;
    int height=0;
    //getData
    AVLNode(const T& data, AVLNode *lt, AVLNode *rt, int h);
};

template<typename T>
AVLNode<T>::AVLNode(const T &d, AVLNode *lt, AVLNode *rt, int h) {
    data(d);
    left(lt);
    right(rt);
    height(h);
}


#endif //SEARCH_ENGINE_AVLNODE_H
