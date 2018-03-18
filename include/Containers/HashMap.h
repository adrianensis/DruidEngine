#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "List.h"
#include "Array.h"
#include <string>
#include <functional>

namespace DE {

/*!
    \brief HashMap of elements.
    \tparam K Key class.
    \tparam V Value class.
*/
template <class K, class V>
class HashMap : public Container {

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
        Node* node = static_cast<Node*>(mAllocator->allocate(smNodeSize));
        node->init(key, element);
        return node;
    };

    void freeNode(Node* node){
        mAllocator->free(node);
    };

    // generic function: arithmetic keys and pointers.
    u64 hash(const u64 key) const {
        return key % mArray->getLength();
    };

    u64 hash(const void* key) const {
        return hash(reinterpret_cast<u64>(key)); // calls hash(const u64 key) const.
    };

    u64 hash(const f32 key) const {
        return hash(static_cast<u64>(key)); // calls hash(const u64 key) const.
    };

    u64 hash(const u8 key) const {
        return hash(static_cast<u64>(key)); // calls hash(const u64 key) const.
    };

    u64 hash(const u16 key) const {
        return hash(static_cast<u64>(key)); // calls hash(const u64 key) const.
    };

    u64 hash(const u32 key) const {
        return hash(static_cast<u64>(key)); // calls hash(const u64 key) const.
    };

    u64 hash(const i8 key) const {
        return hash(static_cast<u64>(key)); // calls hash(const u64 key) const.
    };

    u64 hash(const i16 key) const {
        return hash(static_cast<u64>(key)); // calls hash(const u64 key) const.
    };

    u64 hash(const i32 key) const {
        return hash(static_cast<u64>(key)); // calls hash(const u64 key) const.
    };

    u64 hash(const i64 key) const {
        return hash(static_cast<u64>(key)); // calls hash(const u64 key) const.
    };

    // for Hash objects
    u64 hash(const Hash key) const {
        return hash(key.hash()); // calls hash(const u64 key) const.
    };

    // for strings
    u64 hash(const std::string key) const {
        std::hash<std::string> hash_fn;
        u64 hashString = hash_fn(key);
        return hash(hashString); // calls hash(const u64 key) const.
    };

    u64 hash(const char* key) const {
        std::string str(key);
        std::hash<std::string> hash_fn;
        u64 hashString = hash_fn(str);
        return hash(hashString); // calls hash(const u64 key) const.
    };

    Array<List<Node*>*>* mArray;

public:

    /*!
        \brief Constructor.
    */
    void init() {
        Container::init(0, sizeof(V), 1);
        mArray = DE::allocate< Array<List<Node*>*> >(*mAllocator);
        mArray->init(100);

        // check class
        bool class_ok = std::is_base_of<Hash, K>::value || std::is_same<K, std::string>::value || std::is_arithmetic<K>::value || std::is_pointer<K>::value;
        DE_ASSERT(class_ok, "K must be integer, pointer or extend Hash class.");

    };

    void set(const K key, const V element) {

        u64 hashIndex = HashMap::hash(key);

        List<Node*>* list = mArray->get(hashIndex);

        // if there is no list, create it
        if(list == nullptr){
            list = DE::allocate< List<Node*> >(*mAllocator);
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
            mLength++;
        }
    };

    V get(const K key) const {

        u64 hashIndex = HashMap::hash(key);

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

        u64 hashIndex = HashMap::hash(key);

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
            mLength--;
        }else
            DE_ASSERT(false, "Can't find the element with given key.");
    };

};


} /* namespace DE */

#endif /* HASHMAP_H_ */
