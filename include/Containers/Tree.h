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
class Tree : public BaseContainer {

private:

    class Node {

    public:
        Node* mParent;
        T mElement;
        Array<Node*>* mChildren;
        u64 mHash;

        Node() = default;
        ~Node() = default;

        void init(Node* parent, const T element, Allocator* allocator, u32 childrenCount) {
            mParent = parent;
            mElement = element;
            mHash = Hash::hash(element);
            mChildren = DE::allocate< Array<Node*> >(*allocator);
            mChildren->init(childrenCount);
        };


        void addChild(Node* node){
            u32 index = Hash::hash(node->mElement) < mHash ? 0 : 1;

            Node* child = mChildren->getLength() == index + 1 ? mChildren->get(index) : nullptr;

            if(child == nullptr) {
                node->mParent = this;
                mChildren->set(index, node);
            } else
                child->addChild(node);
        };
    };

    static const u32 smNodeSize = sizeof(Node);

    Node* newNode(Node* parent, const T element){
        Node* node = static_cast<Node*>(mAllocator->allocate(smNodeSize));
        node->init(parent, element, mAllocator, mChildrenCount);
        return node;
    };

    void freeNode(Node* node){
        mAllocator->free(node);
    };

    void freeSubTree(Node* node) {

        Node* child0 = node->mChildren->getLength() == 1 ? node->mChildren->get(0) : nullptr;
        Node* child1 = node->mChildren->getLength() == 2 ? node->mChildren->get(1) : nullptr;

        if(child0 != nullptr)
            freeSubTree(child0);

        if(child1 != nullptr)
            freeSubTree(child1);

        // mAllocator->free(node->mChildren);
        // mAllocator->free(node);

        mLength--;
    };

    Node* find(const T element, Node* node){

        if(element == node->mElement)
            return node;

        u32 index = Hash::hash(element) < node->mHash ? 0 : 1;

        Node* child = node->mChildren->getLength() == index + 1 ? node->mChildren->get(index) : nullptr;

        if(element != node->mElement && child == nullptr){
            return nullptr;
        }else
            return find(element, child);
    };

    Node* minNode(Node* node){
        u32 minv = node->mHash;
        while (node->mChildren->getLength() > 0 && node->mChildren->get(0) != nullptr) {
            minv = node->mChildren->get(0)->mHash;
            node = node->mChildren->get(0);
        }

        return node;
    }

    Node* mRoot;
    static const u32 mChildrenCount = 2;

public:

    /*!
        \brief Constructor.
    */
    void init() {
        BaseContainer::init(0, sizeof(T), 1);
        mRoot = nullptr;
    };

    void add(const T element) {
        Node* node = newNode(nullptr, element);

        if(mRoot == nullptr)
            mRoot = node;
        else
            mRoot->addChild(node);

        mLength++;
    };

    void remove(const T element) {
        Node* node = find(element, mRoot);

        if(node != nullptr){

            Node* child0 = node->mChildren->getLength() == 1 ? node->mChildren->get(0) : nullptr;
            Node* child1 = node->mChildren->getLength() == 2 ? node->mChildren->get(1) : nullptr;

            if(child0 == nullptr && child1 == nullptr){ // 1. If no children
                mAllocator->free(node->mChildren);
                mAllocator->free(node);

            }else if(child0 != nullptr && child1 == nullptr){ // 2.0. If only 1 child
                node->mParent->mChildren->set(0, child0);

            }else if(child0 == nullptr && child1 != nullptr){ // 2.1. If only 1 child
                node->mParent->mChildren->set(1, child1);

            }else{ // 3. If 2 children

                Node* suc = minNode(node->mChildren->get(1)); // inorderSuccessor

                node->mHash = suc->mHash;
                node->mElement = suc->mElement;

                remove(suc->mElement);
            }

            if(node->mParent != nullptr){
                u32 index = node->mHash < node->mParent->mHash ? 0 : 1;
                node->mParent->mChildren->set(index,nullptr);
            }

            mLength--;
        }
    };

    void clear() override {
        Node* current = mRoot;

        freeSubTree(mRoot);
    };

};


} /* namespace DE */

#endif /* TREE_H_ */
