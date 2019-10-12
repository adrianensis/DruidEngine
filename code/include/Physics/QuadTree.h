#ifndef DE_QUADTREE_H
#define DE_QUADTREE_H

#include "DE_Class.h"
#include "Vector2.h"
#include "Vector3.h"

namespace DE {

  class Collider;
  enum class ColliderStatus;
  template<class T> class List;
  template<class T> class Array;
  template <class K, class V> class HashMap;

class QuadTree : public DE_Class{
private:

  class Node {
  public:
    Vector2 mLeftTop;
  	f32 mWidth;
  	f32 mHeight;
  	f32 mMinWidth;
  	f32 mMinHeight;
  	QuadTree* mTree;

    f32 mHalfWidth;
    f32 mHalfHeight;
    bool mIsDivisible;

    List<Collider*>* mColliders;
    Array<Node*>* mChildren;
    Array<Vector2>* mLeftTopChildrenArray;

    u32 mChildrenCount;

    Node();
    ~Node();
    void init(const Vector2& leftTop, f32 width, f32 height, f32 minWidth, f32 minHeight, QuadTree* tree);
    Node* createChildNode(u32 index);
    void addCollider(Collider* collider);
    void update(/*contactManager*/);
    void updateChildren(/*contactManager*/) ;
    bool checkExit(Collider* collider) const ;
    void manageExits(/*exitColliders*/);

    bool isLeaf() const ;
    u32 getCollidersCount() const ;
    bool testCompleteCollider(Collider* collider) const ;
    bool testPartialCollider(Collider* collider) const ;
    bool childNodeTestPartialCollider(u32 index, Collider* collider) const ;

  };

  class Contact {
  public:

    Collider* mColliderA;
    Collider* mColliderB;

    Vector3 mContactPoint;
    Vector3 mNormal; // From B to A
    Vector3 mRelativeVelocity;
    f32 mDepth;
    bool mSolved;
    //mAlive = 0;
  };

  class ContactsManager {
  public:
    // ID = ContactID = ColliderA + Collider B
    // HashMap<ID, Contact>

    // TODO : ID should be just the pointer or hash
  };

  Node* mRoot;

  ContactsManager* mContactsManager;

  f32 mWidth;
  f32 mHeight;

  ColliderStatus mStatus;

public:

  DE_CLASS(QuadTree, DE_Class);

  void init(f32 size);

  void addCollider(Collider* collider);
  void update();

  ColliderStatus getStatus() const;
  void setStatus(ColliderStatus status);

};
} /* namespace DE */
#endif /* DE_QUADTREE_H */
