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

    Node() = default;
    ~Node() = default;

    void init(Node* parent, const T element, Allocator* allocator, u32 childrenCount) {
      mParent = parent;
      mElement = element;
      mChildren = DE::allocate< Array<Node*> >(*allocator);
      mChildren->init(childrenCount);
    };


    void addChild(Node* node){
      u32 index = Hash::hash(node->mElement) < Hash::hash(mElement) ? 0 : 1;

      Node* child = mChildren->get(index);

      if(child == nullptr) {
        node->mParent = this;
        mChildren->set(index, node);
      } else
        child->addChild(node);
    };

    void removeChild(Node* node){
      u32 index = Hash::hash(node->mElement) < Hash::hash(mElement) ? 0 : 1;

      mChildren->get(index)->mParent = nullptr;
      mChildren->set(index,nullptr);
    }
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
    Node* child0 = node->mChildren->get(0);
    Node* child1 = node->mChildren->get(1);

    if(child0 != nullptr)
      freeSubTree(child0);

    if(child1 != nullptr)
      freeSubTree(child1);

    node->mChildren->set(0, nullptr);
    node->mChildren->set(1, nullptr);

    // mAllocator->free(node->mChildren);
    // mAllocator->free(node);

    mLength--;
  };

  Node* find(const T element, Node* node){
    if(element == node->mElement)
      return node;

    u32 index = Hash::hash(element) < Hash::hash(node->mElement) ? 0 : 1;

    Node* child = node->mChildren->get(index);

    if(child == nullptr)
      return nullptr;
    else
      return find(element, child);
  };

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

      Node* child0 = node->mChildren->get(0);
      Node* child1 = node->mChildren->get(1);

      // 3 cases: a, b, c.

      if(child0 == nullptr && child1 == nullptr){ // a) If no children
        // mAllocator->free(node->mChildren);
        // mAllocator->free(node);

        Node* parent = node->mParent;
        node->mParent = nullptr;

        if(parent != nullptr)
          parent->removeChild(node);
        else// Is the root!
          mRoot = nullptr;

      }else if(child0 != nullptr && child1 != nullptr){ // b) If 2 children
        Node* inorderSuccessor; // inorderSuccessor
        Node* inorderSuccessorParent = node->mChildren->get(1);
        while (inorderSuccessorParent->mChildren->getLength() > 0 && inorderSuccessorParent->mChildren->get(0) != nullptr){
          inorderSuccessor = inorderSuccessorParent->mChildren->get(0);
          inorderSuccessorParent = inorderSuccessor;
        }

        inorderSuccessorParent->mChildren->set(0,nullptr);

        node->mElement = inorderSuccessor->mElement;

        // TODO: delete inorderSuccessor node

      }else{ // c) If only 1 child
        Node* child = child0 != nullptr ? child0 : child1;
        Node* parent = node->mParent;
        node->mParent = nullptr;

        if(parent != nullptr){
          parent->removeChild(node);
          parent->addChild(child);
        }else{ // Is the root!
          child->mParent = nullptr;
          mRoot = child;
        }
      }

      mLength--;
    }
  };

  void clear() override {
    if(mLength > 0){
      freeSubTree(mRoot);
    }
    mRoot = nullptr;
  };
};

} /* namespace DE */

#endif /* TREE_H_ */
