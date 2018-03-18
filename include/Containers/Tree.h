#ifndef TREE_H_
#define TREE_H_

#include "Array.h"

namespace DE {

/*!
    \brief Tree of elements with k-children per node.
    \tparam K Key class.
    \tparam T Talue class.
*/
template <class T>
class Tree : public Container {

private:

    class Node {

    public:
        Node* mParent;
        T mElement;
        Array<Node*> mChildren;

        Node() { };
        ~Node() { };

        void init(const Node* parent, const T element) {
            mParent = parent;
            mElement = element;
        };
    };

    static const u32 smNodeSize = sizeof(Node);

    Node* newNode(const Node* parent, const T element){
        Node* node = static_cast<Node*>(mAllocator->allocate(smNodeSize));
        node->init(parent, element);
        return node;
    };

    void freeNode(Node* node){
        mAllocator->free(node);
    };

    Node* root;

public:

    /*!
        \brief Constructor.
    */
    void init() {

    };

    void add(const T element) {

    };

    T remove(const T key) {

    };

};


} /* namespace DE */

#endif /* TREE_H_ */
