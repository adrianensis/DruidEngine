#include "QuadTree.h"
#include "Collider.h"
#include "List.h"
#include "Array.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Memory.h"
#include "Debug.h"
#include "RenderEngine.h"

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

  FOR_ARRAY(i, mChildren){
    Memory::free<Node>(mChildren->get(i));
  }

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

  mExitingColliders = Memory::allocate<List<Collider*>>();
  mExitingColliders->init();

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

	FOR_ARRAY (i, vertices) {
		collision = collision && Collider::testRectanglePoint(mLeftTop,mWidth,mHeight,vertices->get(i),0);
	}

	return collision;
};

//----------------------------------------------------------------------

bool QuadTree::Node::testPartialCollider(Collider* collider) const {

  Array<Vector2>* vertices = collider->getBoundingBox();

	bool collision = false;

	FOR_ARRAY_COND(i, vertices, !collision){
		collision = Collider::testRectanglePoint(mLeftTop,mWidth,mHeight,vertices->get(i),0);
	}

	return collision;
};

//----------------------------------------------------------------------

bool QuadTree::Node::childNodeTestPartialCollider(u32 index, Collider* collider) const {

	Array<Vector2>* vertices = collider->getBoundingBox();

	bool collision = false;

	// For each collider vertex
	FOR_ARRAY_COND(i, vertices, !collision){
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

  // ECHO("addCollider");
  if(mIsDivisible){
    // ECHO("Is Divisible");

    // For each "possible" child node
    FOR_ARRAY (i, mLeftTopChildrenArray){

      bool isPartiallyInChildren = childNodeTestPartialCollider(i,collider);

      if( /*( ! collider.isStatic()) ||*/ isPartiallyInChildren){
        if(isPartiallyInChildren){

          // ECHO("Collider isPartiallyInChildren");

          // If child doesn't exist, create it.
          if(! mChildren->get(i)){
            // ECHO("createChildNode");
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

      FOR_LIST_COND (it, mColliders, !found) {
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

  mExitingColliders->clear(); // colliders which have left the node.

  // DEBUG DRAW
  RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x, mLeftTop.y,0), Vector3(mLeftTop.x, mLeftTop.y - mHeight,0));
  RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x, mLeftTop.y - mHeight,0), Vector3(mLeftTop.x + mWidth, mLeftTop.y - mHeight,0));
  RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x + mWidth, mLeftTop.y - mHeight,0), Vector3(mLeftTop.x + mWidth, mLeftTop.y,0));
  RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x + mWidth, mLeftTop.y,0), Vector3(mLeftTop.x, mLeftTop.y,0));

	// If is leaf node.
	if(isLeaf()){

		// FOR EACH COLLIDER
		FOR_LIST(itA, mColliders){

      Collider* colliderA = itA.get();

			// if collider has left the node.
			if(checkExit(colliderA)) {
        mExitingColliders->pushBack(colliderA);
      }

			// if there are 2 or more colliders within the same node.
			if(mColliders->getLength() > 1){

				// CHECK COLLISIONS WITH THE OTHERS COLLIDERS
        FOR_LIST(itB, mColliders){

          Collider* colliderB = itB.get();

					// if they aren't the same collider
					if(colliderA != colliderB){

						// check bounding radius
						if(colliderA->checkCollisionRadius(colliderB)){

	 						// candidate vertices
	 						// Array<Vector2>* vertices = colliderA.getCandidateVertices(colliderB);
	 						//Array<Vector2>* candidateVertices = colliderA->getBoundingBox();

	 						// Compute candidates and generate contacts
	 						//ColliderStatus status = colliderA->generateContacts(candidateVertices, colliderB/*, contactManager*/);

	 						ColliderStatus status = colliderA->testRectangleRectangle(colliderB);

              // if(status == ColliderStatus::STATUS_PENETRATION) ECHO("penetration");

	 						// console.log(this.tree.getStatus());
	 						// if(mTree->getStatus() != ColliderStatus::STATUS_PENETRATION && status != ColliderStatus::STATUS_NONE){
	 						if(mTree->getStatus() == ColliderStatus::STATUS_NONE){
                //ECHO("STATUS_PENETRATION");
                mTree->setStatus(status);
              }

  					}
          }
				}
			}
		}

	 	manageExits(mExitingColliders);

	}else{
		updateChildren(/*contactManager*/);
	}
};

//----------------------------------------------------------------------

void QuadTree::Node::updateChildren(/*contactManager*/) {
	FOR_ARRAY (i, mChildren){
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
	return ( /*! collider.isStatic() &&*/ ! testCompleteCollider(collider));
};

//----------------------------------------------------------------------

void QuadTree::Node::manageExits(List<Collider*>* exitingColliders) {

 	// If any collider has left the node
 	if(exitingColliders->getLength() > 0){

    // FOR EACH COLLIDER
    FOR_LIST (itExiting, exitingColliders){
      mColliders->remove(mColliders->find(itExiting.get()));
      mTree->addCollider(itExiting.get());
    }
 	}
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
  mStatus = ColliderStatus::STATUS_NONE;
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
	mRoot->init(Vector2(-mWidth/2.0f, mHeight/2.0f), mWidth, mHeight, 500.0f, 500.0f, this);
}

// ---------------------------------------------------------------------------

void QuadTree::update() {
  mStatus = ColliderStatus::STATUS_NONE;
  mRoot->update();
}

// ---------------------------------------------------------------------------

void QuadTree::addCollider(Collider* collider) {
  mRoot->addCollider(collider);
}

// ---------------------------------------------------------------------------


} /* namespace DE */
