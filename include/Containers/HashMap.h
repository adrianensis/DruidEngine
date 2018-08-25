#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "List.h"
#include "Array.h"
#include <functional>

namespace DE {

/*!
  \brief HashMap of elements.
  \tparam K Key class.
  \tparam V Value class.
*/
template <class K, class V>
class HashMap : public BaseContainer {

private:

  class Node {

  public:
    K mKey;
    V mElement;

    Node() { };
    ~Node() { };

    void init(const K key, const V element) {
      mKey = key;
      mElement = element;
      };
  };

  static const u32 smNodeSize = sizeof(Node);

  Node* newNode(const K key, const V element){
    Node* node = static_cast<Node*>(BaseContainer::mAllocator->allocate(smNodeSize));
    node->init(key, element);
    return node;
  };

  void freeNode(Node* node){
    BaseContainer::mAllocator->free(node);
  };

  Array<List<Node*>*>* mArray;

public:

  /*!
    \brief Constructor.
  */
  void init() {
    BaseContainer::init(0, sizeof(V), 1);
    mArray = DE::allocate< Array<List<Node*>*> >(*BaseContainer::mAllocator);
    mArray->init(100);

    // check class
    bool class_ok = std::is_base_of<Hash, K>::value || std::is_same<K, std::string>::value || std::is_arithmetic<K>::value || std::is_pointer<K>::value;
    DE_ASSERT(class_ok, "K must be integer, pointer or extend Hash class.");

  };

  void set(const K key, const V element) {

    u64 hashIndex = Hash::hash(key) % mArray->BaseContainer::getLength();

    List<Node*>* list = mArray->get(hashIndex);

    // if there is no list, create it
    if(list == nullptr){
      list = DE::allocate< List<Node*> >(*BaseContainer::mAllocator);
      list->init();
      mArray->set(hashIndex, list);
    }

    auto it = list->getIterator();
    bool found = false;
    Node* node = nullptr;

    for (; !it.isNull() && !node; it.next()){
      if(it.get()->mKey == key){
        node = it.get();
      }
    }

    // if element exists, update element
    if(node)
      node->mElement = element;
    else{
      list->pushBack(newNode(key, element));
      BaseContainer::mLength++;
    }
  };

  V get(const K key) const {

    u64 hashIndex = Hash::hash(key) % mArray->BaseContainer::getLength();

    // iterate over list to find element.

    auto list = mArray->get(hashIndex);

    bool found = false;
    V element;

    if(list != nullptr){

      auto it = list->getIterator();

      for (; !it.isNull() && !found; it.next()){
        if(it.get()->mKey == key){
          found = true;
          element = it.get()->mElement;
        }
      }
    }

    if(found)
      return element;
    else
      DE_ASSERT(false, "Can't find the element with given key.");
  };

  V remove(const K key) {

    u64 hashIndex = Hash::hash(key) % mArray->BaseContainer::getLength();

    auto list = mArray->get(hashIndex);

    bool found = false;
    u32 index = 0;

    if(list != nullptr){

      // iterate over list to find element.
      auto it = list->getIterator();

      for (; !it.isNull() && !found; it.next()){
        if(it.get()->mKey == key)
        found = true;

        index++;
      }
    }

    if(found){
      list->remove(index-1);
      BaseContainer::mLength--;
    }else
      DE_ASSERT(false, "Can't find the element with given key.");
  };

  void clear() override {
    // TODO: implement
  };
};

} /* namespace DE */

#endif /* HASHMAP_H_ */
