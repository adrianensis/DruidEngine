#include "QuadTree.h"
#include "Collider.h"
#include "List.h"
#include "Array.h"
#include "Vector2.h"
#include "Memory.h"
#include "Debug.h"

namespace DE {

// ---------------------------------------------------------------------------

QuadTree::Node::Node() {
  mTree = nullptr;
  mColliders = nullptr;
  mChildren = nullptr;
  mLeftTopChildrenArray = nullptr;
  mChildrenCount = 0;
}

// ---------------------------------------------------------------------------

QuadTree::Node::~Node() {
  mTree = nullptr;
  Memory::free<List<Collider*>>(mColliders);
}

// ---------------------------------------------------------------------------

void QuadTree::Node::init(const Vector2& leftTop, f32 width, f32 height, f32 minWidth, f32 minHeight, QuadTree* tree) {
  mLeftTop = leftTop;
  mWidth = width;
  mHeight = height;
  mMinWidth = minWidth;
  mMinHeight = minHeight;
  mTree = tree;

  mHalfWidth = mWidth/2.0f;
  mHalfHeight = mHeight/2.0f;
  mIsDivisible = (mHalfWidth >= mMinWidth) && (mHalfHeight >= mMinHeight);

  mColliders = Memory::allocate<List<Collider*>>();
  mColliders->init();

  mChildren = Memory::allocate<Array<Node*>>();
  mChildren->init(4);

  mLeftTopChildrenArray = Memory::allocate<Array<Vector2>>();
  mLeftTopChildrenArray->init(4);

  // This array testPartialCollider the LeftTop Vertices of the 4 children.
  mLeftTopChildrenArray->set(0, Vector2(mLeftTop.x, mLeftTop.y));
  mLeftTopChildrenArray->set(1, Vector2(mLeftTop.x, mLeftTop.y - mHalfHeight));
  mLeftTopChildrenArray->set(2, Vector2(mLeftTop.x + mHalfWidth, mLeftTop.y - mHalfHeight));
  mLeftTopChildrenArray->set(3, Vector2(mLeftTop.x + mHalfWidth, mLeftTop.y));
}

//----------------------------------------------------------------------


bool QuadTree::Node::isLeaf() const {
	return mChildrenCount == 0;
};

//----------------------------------------------------------------------

u32 QuadTree::Node::getCollidersCount() const {
  return mColliders->getLength();
};

//----------------------------------------------------------------------

bool QuadTree::Node::testCompleteCollider(Collider* collider) const {

	Array<Vector2>* vertices = collider->getBoundingBox();

	bool collision = true;

	for (u32 i = 0; i < vertices->getLength(); i++) {
		collision = collision && Collider::testRectanglePoint(mLeftTop,mWidth,mHeight,vertices->get(i),0);
	}

	return collision;
};

//----------------------------------------------------------------------

bool QuadTree::Node::testPartialCollider(Collider* collider) const {

  Array<Vector2>* vertices = collider->getBoundingBox();

	bool collision = false;

	for (u32 i = 0; i < vertices->getLength() && ! collision; i++) {
		collision = Collider::testRectanglePoint(mLeftTop,mWidth,mHeight,vertices->get(i),0);
	}

	return collision;
};

//----------------------------------------------------------------------

bool QuadTree::Node::childNodeTestPartialCollider(u32 index, Collider* collider) const {

	Array<Vector2>* vertices = collider->getBoundingBox();

	bool collision = false;

	// For each collider vertex
	for (u32 i = 0; i < vertices->getLength() && ! collision; i++) {
		collision = Collider::testRectanglePoint(mLeftTopChildrenArray->get(index),mHalfWidth,mHalfHeight,vertices->get(i),0);
	}

	return collision;
};


//----------------------------------------------------------------------

QuadTree::Node* QuadTree::Node::createChildNode(u32 index){
	mChildrenCount++;
  Node* node= Memory::allocate<Node>();
	node->init(mLeftTopChildrenArray->get(index), mHalfWidth, mHalfHeight, mMinWidth, mMinHeight, mTree);
  return node;
};

// ---------------------------------------------------------------------------

void QuadTree::Node::addCollider(Collider* collider){

  //ECHO("addCollider");
  if(mIsDivisible){
    //ECHO("Is Divisible");

    // For each "posible" child node
    for (u32 i = 0; i < mLeftTopChildrenArray->getLength(); i++){

      bool isPartiallyInChildren = childNodeTestPartialCollider(i,collider);

      if( /*( ! collider.isStatic()) ||*/ isPartiallyInChildren){
        if(isPartiallyInChildren){

          //ECHO("Collider isPartiallyInChildren");

          // If child doesn't exist, create it.
          if(! mChildren->get(i)){
            mChildren->set(i, createChildNode(i));
          }

          mChildren->get(i)->addCollider(collider);
        }
      }
    }
  } else {
    if ( /*( ! collider.isStatic()) || */ testPartialCollider(collider)) {

      ECHO("ADD");
      bool found = false;

      // for (var i = 0; !found && i < mColliders->getLength(); i++) {
      //   found = (mColliders->get(i).getId() == collider.getId());
      // }

      if(!found){
        mColliders->pushBack(collider);
        ECHO("ADDED COLLIDER");
      }
    }
  }
}

//----------------------------------------------------------------------

void QuadTree::Node::update(/*contactManager*/){

	// // this.draw();
  //
	// var exitColliders = []; // colliders which have left the node.
  //
	// // If is leaf node.
	// if(this.isLeaf()){
  //
	// 	// console.log(this.colliders.length);
  //
	// 	// FOR EACH COLLIDER
	// 	for (var i = 0; i < this.colliders.length; i++){
  //
	// 		// if collider has left the node.
	// 		if(this.checkExit(this.colliders[i]))
	// 			exitColliders.push(this.colliders[i]);
  //
	// 		// if there are 2 or more colliders within the same node.
	// 		if(this.colliders.length > 1){
  //
	// 			// CHECK COLLISIONS WITH THE OTHERS COLLIDERS
	// 			for (var j= 0; j < this.colliders.length; j++){
  //
	// 				// ifthey aren't the same collider
	// 				if(this.colliders[i].getId() !== this.colliders[j].getId()){
  //
	// 					// check bounding radius
	// 					if(this.colliders[i].checkCollisionRadius(this.colliders[j])){
  //
	// 						// candidate vertices
	// 						var vertices = this.colliders[i].getCandidateVertices(this.colliders[j]);
  //
	// 						// Compute candidates and generate contacts
	// 						// var contacts = [];// FIXME: DEPRECATED
	// 						var status = this.colliders[i].generateContacts(vertices, this.colliders[j], contactManager);
  //
  //
	// 						// this.tree.addContacts(contacts);
  //
	// 						// console.log(this.tree.getStatus());
	// 						if(this.tree.getStatus() !== Collider.STATUS_PENETRATION && status !== Collider.STATUS_NONE)
	// 							this.tree.setStatus(status);
  //
	// 						// if(status === Collider.STATUS_NONE)
	// 						// 	contactManager.remove(this.colliders[i],this.colliders[j]);
  //
	// 					}
	// 					// else{
	// 					// 	contactManager.remove(this.colliders[i],this.colliders[j]);
	// 					// }
	// 				}
	// 			}
	// 		}
	// 	}
  //
  //
	// 	this.manageExits(exitColliders);
  //
	// }else{
	// 	this.updateChildren(contactManager);
	// }
};

//----------------------------------------------------------------------
void QuadTree::Node::updateChildren(/*contactManager*/) {
	// // for (var i = 0; i < this.children.length; i++)
	// for (var i = 0; i < this.children.length; i++){
  //
	// 	var child = this.children[i];
  //
	// 	if(child !== null){
  //
	// 		if(child.isLeaf() && child.getCollidersCount() === 0){
	// 			this.children[i] = null;
	// 			this.enabledChildren--;
	// 		}else{
	// 			child.update(contactManager);
	// 		}
	// 	}
	// }
};

//----------------------------------------------------------------------

bool QuadTree::Node::checkExit(Collider* collider) const {
	// CHECK if collider is out of this node.
	// only dynamic objects can escape from their nodes !!!
	//return ( ! collider.isStatic() && ! this.testCompleteCollider(collider));

  return false;
};

//----------------------------------------------------------------------

void QuadTree::Node::manageExits(/*exitColliders*/){
//
// 	// If any collider has left the node
// 	if(exitColliders.length > 0){
//
// 		var remainingColliders = [];
//
// 		// for each collider
// 		for (var i = 0; i < this.colliders.length; i++) {
//
// 			var erased = false;
//
// 			// check if the collider has left the nodes
// 			for (var j = 0; j < exitColliders.length && !erased ; j++) {
// 				if(exitColliders[j].getId() === this.colliders[i].getId()){
// 					erased = true;
// 				}
// 			}
//
// 			if(!erased){
// 				remainingColliders.push(this.colliders[i]);
// 			}
// 		}
//
// 		this.colliders = remainingColliders;
//
// 		// RE-INSERT
// 		for (var i = 0; i < exitColliders.length; i++) {
// 			this.tree.addCollider(exitColliders[i]);
// 		}
// 	}
};


// ---------------------------------------------------------------------------

QuadTree::QuadTree() : DE_Class(){
  mRoot = nullptr;
}

// ---------------------------------------------------------------------------

QuadTree::~QuadTree(){
  Memory::free<Node>(mRoot);
}

// ---------------------------------------------------------------------------

void QuadTree::init(f32 size){
  mWidth = size;
  mHeight = size;

  mRoot = Memory::allocate<Node>();
	mRoot->init(Vector2(-mWidth/2.0f, mHeight/2.0f), mWidth, mHeight, 100.0f, 100.0f, this);
}

// ---------------------------------------------------------------------------

void QuadTree::addCollider(Collider* collider) {
  mRoot->addCollider(collider);
}

// ---------------------------------------------------------------------------


} /* namespace DE */
