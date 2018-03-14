#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "Container.h"
#include "List.h"
#include "DynamicArray.h"
#include <string>

namespace DE {

/*!
    \brief HashMap of elements. Dynamic size.
    \tparam K Key class.
    \tparam V Value class.
*/
template <class K, class V>
DE_CLASS(HashMap) DE_CLASS_EXTENDS public Container {

private:

    class Node {

    public:
        K mKey;
        V mElement;

        Node() { };

        ~Node() { };

        void init(K key, V element) {
            mKey = key;
            mElement = element;
        };
    };

    static const u32 smNodeSize = sizeof(Node);

    Node* newNode(K key, V element){
        Node* node = static_cast<Node*>(mAllocator->allocate(smNodeSize));
        node->init(key, element);
        return node;
    };

    void freeNode(Node* node){
        mAllocator->free(node);
    };

    u64 hash(K key){

        u32 arrayLength = mArray->mArrays->getLength();

        u64 hashIndex = -1;

        // if(std::is_base_of<Hash, K>::value) // if it is Hash
        //     hashIndex = key.hash() % arrayLength;
        // else
        if(std::is_arithmetic<K>::value) // if it is number
            hashIndex = key % arrayLength;
        else if(std::is_pointer<K>::value) // if it is pointer
            hashIndex = static_cast<u64>(key) % arrayLength;
        // else if(std::is_same<K, std::string>::value){
        //     std::hash<std::string> hash_fn;
        //     u64 hashString = hash_fn(key);
        //     hashIndex = hashString % arrayLength;
        // }

        else
            ASSERT(false, "K must be integer, pointer or extends Hash.");

        return hashIndex;
    }

    DynamicArray<List<Node*>*>* mArray;

public:

    /*!
        \brief Constructor.
    */
    void init() {
        Container::init(0, sizeof(V), 1);
        mArray = DE::allocate< DynamicArray<List<Node*>*> >(*mAllocator);
        mArray->init();
    };

    void set(K key, V element) {

        u64 hashIndex = HashMap::hash(key);

        // if there is no list, create it
        if(mArray->get(hashIndex) == nullptr){
            List<Node*>* newList = DE::allocate< List<Node*> >(*mAllocator);
            newList->init();
            mArray->set(hashIndex, newList);
        }

        // add new node to list
        Node* node = newNode(key, element);
        mArray->get(hashIndex)->pushBack(node);
    };

    V get(K key) {

        u64 hashIndex = HashMap::hash(key);

        // iterate over list to find element.
        auto it = mArray->get(hashIndex)->getIterator();
        bool found = false;

        for (; !it.isNull() && !found; it.next()){
            Node* node = it.get();

            if(node->mKey == key)
                found = true;
        }

        if(found){
            it.prev();
            return it.get();
        }else{
            ASSERT(false, "Can't find the element with given key.");
        }
    };

};


} /* namespace DE */

#endif /* HASHMAP_H_ */
