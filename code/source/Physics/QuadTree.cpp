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
    mLeftTop = Vector2();
    mWidth = 0.0f;
    mHeight = 0.0f;
    mMinWidth = 0.0f;
    mMinHeight = 0.0f;
    mHalfWidth = 0.0f;
    mHalfHeight = 0.0f;
    mIsDivisible = false;
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
    //if ( /*( ! collider.isStatic()) */) {

      //ECHO("ADD");
      bool found = false;

      for (auto it = mColliders->getIterator(); !found && !it.isNull(); it.next()) {
        found = (it.get() == collider);
      }

      if(!found){
        mColliders->pushBack(collider);
        //ECHO("ADDED COLLIDER");
      }
    //}
  }
}

//----------------------------------------------------------------------

void QuadTree::Node::update(/*contactManager*/){

	// var exitColliders = []; // colliders which have left the node.
  //
	// If is leaf node.
	if(isLeaf()){
    //ECHO("IS LEAF");
		// FOR EACH COLLIDER
		for (auto itA = mColliders->getIterator(); !itA.isNull(); itA.next()){

      Collider* colliderA = itA.get();

			// if collider has left the node.
			if(checkExit(colliderA)){
				//exitColliders.push(colliderA);
      }

			// if there are 2 or more colliders within the same node.
			if(mColliders->getLength() > 1){

				// CHECK COLLISIONS WITH THE OTHERS COLLIDERS
        for (auto itB = mColliders->getIterator(); !itB.isNull(); itB.next()){

          Collider* colliderB = itB.get();

					// if they aren't the same collider
					if(colliderA != colliderB){

						// check bounding radius
						if(colliderA->checkCollisionRadius(colliderB)){

	 						// candidate vertices
	 						// Array<Vector2>* vertices = colliderA.getCandidateVertices(colliderB);
	 						Array<Vector2>* candidateVertices = colliderA->getBoundingBox();

	 						// Compute candidates and generate contacts
	 						ColliderStatus status = colliderA->generateContacts(/*candidateVertices, colliderB, contactManager*/);

	 						// console.log(this.tree.getStatus());
	 						if(mTree->getStatus() != ColliderStatus::STATUS_PENETRATION && status != ColliderStatus::STATUS_NONE)
	 							mTree->setStatus(status);

	 						// if(status === Collider.STATUS_NONE)
	 						// 	contactManager.remove(this.colliders[i],this.colliders[j]);

//	 					}
//	 					// else{
//	 					// 	contactManager.remove(this.colliders[i],this.colliders[j]);
//	 					// }
  					}
          }
				}
			}
		}
  //
  //
	// 	this.manageExits(exitColliders);
  //
	}else{
		updateChildren(/*contactManager*/);
	}
};

//----------------------------------------------------------------------

void QuadTree::Node::updateChildren(/*contactManager*/) {
	// // for (var i = 0; i < this.children.length; i++)
	for (u32 i = 0; i < mChildren->getLength(); i++){
		Node* child = mChildren->get(i);

		if(child){

			if(child->isLeaf() && (child->getCollidersCount() == 0)){
				//this.children[i] = null;
				//this.enabledChildren--;
			}else{
        child->update(/*contactManager*/);
			}
		}
	}
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

//----------------------------------------------------------------------

bool QuadTree::Node::isLeaf() const { return mChildrenCount == 0; };
u32 QuadTree::Node::getCollidersCount() const { return mColliders->getLength(); };

// ---------------------------------------------------------------------------

QuadTree::QuadTree() : DE_Class(){
  mRoot = nullptr;
  mContactsManager = nullptr;
  mWidth = 0.0f;
  mHeight = 0.0f;
}

// ---------------------------------------------------------------------------

QuadTree::~QuadTree(){

  // TODO : First remove mRoot children

  Memory::free<Node>(mRoot);
}

// ---------------------------------------------------------------------------

void QuadTree::init(f32 size){
  mWidth = size;
  mHeight = size;

  mRoot = Memory::allocate<Node>();
	mRoot->init(Vector2(-mWidth/2.0f, mHeight/2.0f), mWidth/2.0f, mHeight/2.0f, 2500.0f, 2500.0f, this);
}

// ---------------------------------------------------------------------------

void QuadTree::update() {
  mRoot->update();
}

// ---------------------------------------------------------------------------

void QuadTree::addCollider(Collider* collider) {
  mRoot->addCollider(collider);
}

// ---------------------------------------------------------------------------

ColliderStatus QuadTree::getStatus() const { return mStatus; }
void QuadTree::setStatus(ColliderStatus status) { mStatus = status; }

// ---------------------------------------------------------------------------


} /* namespace DE */
