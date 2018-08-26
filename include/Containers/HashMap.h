#ifndef DE_HASHMAP_H
#define DE_HASHMAP_H

#include "List.h"
#include "Array.h"
#include "Memory.h"
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

    Node() = default;
    ~Node() = default;

    void init(const K key, const V element) {
      mKey = key;
      mElement = element;
    };
  };

  static const u32 smNodeSize = sizeof(Node);

  Node* newNode(const K key, const V element){
    Node* node = Memory::allocate<Node>();
    node->init(key, element);
    return node;
  };

  void freeNode(Node* node){
    Memory::free<Node>(node);
  };

  Array<List<Node*>*>* mArray;

public:

  /*!
    \brief Default Constructor.
  */
  HashMap(){
    mArray = nullptr;
  };

  /*!
  \brief Destructor.
  */
  ~HashMap(){
    HashMap::clear();
  };

  /*!
    \brief Constructor.
  */
  void init() {
    BaseContainer::init(0, sizeof(V), 1);
    mArray = Memory::allocate<Array<List<Node*>*>>();
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
      list = Memory::allocate< List<Node*>>();
      list->init();
      mArray->set(hashIndex, list);
    }

    auto it = list->getIterator();
    bool found = false;
    Node* node = nullptr;

    for (; !it.isNull() && !found; it.next()){
      if(it.get()->mKey == key){
        found = true;
        node = it.get();
      }
    }

    // if element exists, update element
    if(found)
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
    auto it = list->getIterator();
    typename List<Node*>::Iterator selectedIt;

    if(list != nullptr){
      // iterate over list to find element.
      for (; !it.isNull() && !found; it.next()){
        if(it.get()->mKey == key){
          found = true;
          selectedIt = it;
        }

        index++;
      }
    }

    if(found){
      freeNode(selectedIt.get());
      list->remove(index-1);
      BaseContainer::mLength--;
    }else
      DE_ASSERT(false, "Can't find the element with given key.");
  };

  void clear() override {
    if(mArray != nullptr){
      for (u32 i = 0; i < mArray->getLength(); i++) {
        List<Node*>* list = mArray->get(i);
        if(list != nullptr && ! list->isEmpty()){
          auto it = list->getIterator();

          if(list != nullptr){
            for (; !it.isNull(); it.next()){
              freeNode(it.get());
            }
          }
        }
      }
    }
  };
};

} /* namespace DE */

#endif /* DE_HASHMAP_H */
