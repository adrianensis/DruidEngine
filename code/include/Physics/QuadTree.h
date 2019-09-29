#ifndef DE_QUADTREE_H
#define DE_QUADTREE_H

#include "DE_Class.h"
#include "Vector2.h"

namespace DE {

  class Collider;
  template<class T> class List;
  template<class T> class Array;

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

  Node* mRoot;

  f32 mWidth;
  f32 mHeight;

public:

  DE_CLASS(QuadTree, DE_Class);

  void init(f32 size);

  void addCollider(Collider* collider);

};
} /* namespace DE */
#endif /* DE_QUADTREE_H */
