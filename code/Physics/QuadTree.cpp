#include "Physics/QuadTree.hpp"
#include "Physics/Collider.hpp"
#include "Containers/List.hpp"
#include "Containers/Array.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"
#include "Memory/Memory.hpp"
#include "Log/Log.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Physics/PhysicsEngine.hpp"
#include "Graphics/Renderer.hpp"
#include "Config/EngineConfig.hpp"
#include "ContactsManager.hpp"
#include "Physics/RigidBody.hpp"
#include "Maths/MathUtils.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Profiler/Profiler.hpp"

namespace DE {

QuadTree::Node::Node() : DE_Class() {
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
	mDynamicCollidersCount = 0;
	mStaticCollidersCount = 0;
}

QuadTree::Node::~Node() {
	mTree = nullptr;

	FOR_ARRAY(i, mChildren)
	{
		if (mChildren->get(i)) {
			DE_FREE(mChildren->get(i));
		}
	}

	DE_FREE(mChildren);

	/*FOR_LIST(it, mColliders) {
		if (!(it.get()->isDestroyed() || it.get()->isPendingToBeDestroyed())) {
			it.get()->setDestroyed();
			DE_FREE(it.get());
		}
	}*/

	DE_FREE(mColliders);
	DE_FREE(mExitingColliders);
	DE_FREE(mLeftTopChildrenArray);
}

void QuadTree::Node::init(const Vector2 &leftTop, f32 width, f32 height, f32 minWidth, f32 minHeight, QuadTree *tree) {
	mLeftTop = leftTop;
	mWidth = width;
	mHeight = height;
	mMinWidth = minWidth;
	mMinHeight = minHeight;
	mRadius = sqrt((mWidth * mWidth) + (mHeight * mHeight)) / 2.0f;
	mTree = tree;

	mHalfWidth = mWidth / 2.0f;
	mHalfHeight = mHeight / 2.0f;
	mIsDivisible = (mHalfWidth >= mMinWidth) && (mHalfHeight >= mMinHeight);

	mColliders = DE_NEW<List<Collider*>>();
	mColliders->init();

	mExitingColliders = DE_NEW<List<Collider*>>();
	mExitingColliders->init();

	mChildren = DE_NEW<Array<Node*>>();
	mChildren->init(4);
	mChildren->set(0, nullptr);
	mChildren->set(1, nullptr);
	mChildren->set(2, nullptr);
	mChildren->set(3, nullptr);

	mLeftTopChildrenArray = DE_NEW<Array<Vector2>>();
	mLeftTopChildrenArray->init(4);

	// This array testPartialCollider the LeftTop Vertices of the 4 children.
	mLeftTopChildrenArray->set(0, Vector2(mLeftTop.x, mLeftTop.y));
	mLeftTopChildrenArray->set(1, Vector2(mLeftTop.x, mLeftTop.y - mHalfHeight));
	mLeftTopChildrenArray->set(2, Vector2(mLeftTop.x + mHalfWidth, mLeftTop.y - mHalfHeight));
	mLeftTopChildrenArray->set(3, Vector2(mLeftTop.x + mHalfWidth, mLeftTop.y));

}

//----------------------------------------------------------------------

bool QuadTree::Node::childNodeTestPartialCollider(u32 index, Collider *collider) const {

	bool collision = false;

	collision = MathUtils::testRectangleSphere(mLeftTopChildrenArray->get(index), mHalfWidth, mHalfHeight,
				Vector2(collider->getCenter()), collider->getRadius(), 0);

	return collision;
};

//----------------------------------------------------------------------

QuadTree::Node* QuadTree::Node::createChildNode(u32 index) {
	mChildrenCount++;
	Node* node = DE_NEW<Node>();
	node->init(mLeftTopChildrenArray->get(index), mHalfWidth, mHalfHeight, mMinWidth, mMinHeight, mTree);
	return node;
};

void QuadTree::Node::addCollider(Collider *collider) {

	if (mIsDivisible) {

		// For each "possible" child node
		FOR_ARRAY (i, mChildren) {

			bool isPartiallyInChildren = childNodeTestPartialCollider(i, collider);

			if (isPartiallyInChildren) {

//				ECHO("ADDED")
//				VAR(u32, i)
				// If child doesn't exist, create it.
				if (!mChildren->get(i)) {
					mChildren->set(i, createChildNode(i));
				}

				mChildren->get(i)->addCollider(collider);
			}
		}
	} else {

		bool found = false;

		FOR_LIST_COND (it, mColliders, !found) {
			found = (it.get() == collider);
		}

		if (!found) {
			mColliders->pushBack(collider);

			if (collider->isStatic()) {
				mStaticCollidersCount++;
			} else {
				mDynamicCollidersCount++;
			}
		}
	}
}

//----------------------------------------------------------------------

void QuadTree::Node::internalRemoveColliderFromList(const Iterator *it) {
	auto castedIt = it->cast<Collider*>();
	mColliders->remove(*castedIt);

}

//----------------------------------------------------------------------

void QuadTree::Node::internalFreeCollider(Collider *collider) {
	collider->setDestroyed();
	DE_FREE(collider);
}

//----------------------------------------------------------------------

void QuadTree::Node::update(/*contactManager*/) {

	// If is leaf node.
	if (isLeaf()) {

//		TRACE();

		/*if (mDynamicCollidersCount + mStaticCollidersCount > 0) {
		// DEBUG DRAW
		 RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x, mLeftTop.y,0), Vector3(mLeftTop.x, mLeftTop.y - mHeight,0));
		 RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x, mLeftTop.y - mHeight,0), Vector3(mLeftTop.x + mWidth, mLeftTop.y - mHeight,0));
		 RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x + mWidth, mLeftTop.y - mHeight,0), Vector3(mLeftTop.x + mWidth, mLeftTop.y,0));
		 RenderEngine::getInstance()->drawLine(Vector3(mLeftTop.x + mWidth, mLeftTop.y,0), Vector3(mLeftTop.x, mLeftTop.y,0));

		}*/

		bool nodeInCameraView = RenderEngine::getInstance()->frustumTestSphere(
				Vector3(mLeftTop.x + mHalfWidth, mLeftTop.y - mHalfHeight, 0), mRadius);

		mExitingColliders->clear(); // colliders which have left the node.

		ColliderStatus newTreeStatus = ColliderStatus::STATUS_NONE;

		// FOR EACH COLLIDER
		FOR_LIST(itA, mColliders) {

			Collider* colliderA = itA.get();

			if(PhysicsEngine::getInstance()->getDebugColliders()) {
				colliderA->render();
			}

			if (colliderA->isActive()) {
				if (colliderA->isSimulate()) {

					bool isStaticA = colliderA->isStatic();

					// check if collider has left the node.
					checkExit(colliderA);

					// if there are 2 or more colliders within the same node
					if (mDynamicCollidersCount + mStaticCollidersCount > 1) {
						// CHECK COLLISIONS WITH THE OTHERS COLLIDERS
						FOR_LIST(itB, mColliders)
						{

							Collider* colliderB = itB.get();

							if (colliderB->isActive()) {
								bool isStaticB = colliderA->isStatic();

								bool sameLayer = colliderA->getCollisionLayer() == colliderB->getCollisionLayer();
								bool bothAreSleeping = colliderA->getRigidBody()->isSleeping() && colliderB->getRigidBody()->isSleeping();

								if (colliderB->isSimulate() && (colliderA != colliderB) && sameLayer && !(isStaticA && isStaticB) && !bothAreSleeping) {

									// check bounding radius
									if (colliderA->checkCollisionRadius(colliderB)) {

										//ECHO("TEST")

										// candidate vertices
										// Array<Vector2>* vertices = colliderA.getCandidateVertices(colliderB);
										//Array<Vector2>* candidateVertices = colliderA->getBoundingBox();

										// Compute candidates and generate contacts
										//ColliderStatus status = colliderA->generateContacts(candidateVertices, colliderB/*, contactManager*/);

										ColliderStatus status = colliderA->testCollider(colliderB);
										Contact* contact = colliderA->getLastContact();

										if(status < ColliderStatus::STATUS_PENETRATION){
											ColliderStatus status2 = colliderB->testCollider(colliderA);
											if(status < status2){
												status = status2;
												contact = colliderB->getLastContact();
											}
										}

										if (status == ColliderStatus::STATUS_NONE /*&& status2 == ColliderStatus::STATUS_NONE*/) {
											//ContactsManager::getInstance()->removeContact(colliderA, colliderB);
										} else {

											if (status == ColliderStatus::STATUS_PENETRATION) {

												if (colliderA->getIsSolid() && colliderB->getIsSolid()) {
													colliderA->markPenetratedBy(colliderB);
													colliderB->markPenetratedBy(colliderA);

													//colliderA->getGameObject()->getComponents<Renderer>()->get(0)->setColor(Vector4(1,0,0,1));
													//colliderB->getGameObject()->getComponents<Renderer>()->get(0)->setColor(Vector4(1,0,0,1));
												}
											}

											if (status == ColliderStatus::STATUS_COLLISION) {
												//colliderA->getGameObject()->getComponents<Renderer>()->get(0)->setColor(Vector4(1,1,0,1));
												//colliderB->getGameObject()->getComponents<Renderer>()->get(0)->setColor(Vector4(1,1,0,1));
											}

											ContactsManager::getInstance()->addContact(contact);
										}

										if (status > newTreeStatus) {
											newTreeStatus = status;
										}

									} else {
										//VAR(u64, (u64)this)
										//ContactsManager::getInstance()->removeContact(colliderA, colliderB);
									}
								}
							} else if (colliderB->getIsPendingToBeDestroyed()) {
								internalRemoveColliderFromList(&itB);
								//internalFreeCollider(colliderB);
							} else if (colliderB->getIsDestroyed()) {
								internalRemoveColliderFromList(&itB);
							}
						}
					}
				}
			} else if (colliderA->getIsPendingToBeDestroyed()) {
				internalRemoveColliderFromList(&itA);
				//internalFreeCollider(colliderA);
			} else if (colliderA->getIsDestroyed()) {
				internalRemoveColliderFromList(&itA);
			}
		}

		if (newTreeStatus > mTree->getStatus()) {
			//ECHO("STATUS_PENETRATION");
			mTree->setStatus(newTreeStatus);
		}

		manageExits(mExitingColliders);

	} else {
		updateChildren(/*contactManager*/);
	}
};

//----------------------------------------------------------------------

void QuadTree::Node::updateChildren(/*contactManager*/) {
	FOR_ARRAY (i, mChildren) {
		Node* child = mChildren->get(i);

		if (child) {
			child->update(/*contactManager*/);
		}
	}
};

//----------------------------------------------------------------------

void QuadTree::Node::checkExit(Collider *collider) const {
	// CHECK if collider is out of this node.
	// only dynamic objects can escape from their nodes !!!

	if (!collider->isStatic() || collider->getHasSizeChanged()) {

//		bool collision = MathUtils::testRectangleSphere(mLeftTop, mWidth, mHeight,
//						Vector2(collider->getCenter()), collider->getRadius(), 0);

		bool collision = MathUtils::testRectanglePoint(mLeftTop, mWidth, mHeight,
								Vector2(collider->getCenter()), -collider->getRadius());

		if(!collision || collider->getHasSizeChanged()){
			mExitingColliders->pushBack(collider);
			mTree->addCollider(collider);
		}
	}
};

//----------------------------------------------------------------------

void QuadTree::Node::manageExits(List<Collider*> *exitingColliders) {

	// If any collider has left the node
	if (exitingColliders->getLength() > 0) {

		// FOR EACH COLLIDER
		FOR_LIST (itExiting, exitingColliders)
		{
			Collider* collider = itExiting.get();
			mColliders->remove(mColliders->find(collider));

			// Note: Only dynamic colliders can leave the node!
			if (!collider->isStatic()) {
				mDynamicCollidersCount--;
			}
		}
	}
};

//----------------------------------------------------------------------

bool QuadTree::Node::isLeaf() const {
	return mChildrenCount == 0;
};
u32 QuadTree::Node::getCollidersCount() const {
	return mColliders->getLength();
};

void QuadTree::Node::rayCastQuery(const Vector3 &lineStart, const Vector3 &lineEnd, List<GameObject*> *outList) {

	bool rayIntersectsNode = MathUtils::testLineSphereSimple(Vector2(lineStart), Vector2(lineEnd),
			Vector2(mLeftTop + Vector3(mHalfWidth, -mHalfHeight, 0)), mRadius, 0.0f);

	if (rayIntersectsNode) {
		if (isLeaf()) {

			FOR_LIST(it, mColliders) {
				Collider* collider = it.get();

				GameObject* gameObject = collider->getGameObject();

				// TODO : line vs rectangle
				bool rayIntersectsCollider = MathUtils::testLineSphereSimple(Vector2(lineStart), Vector2(lineEnd),
						Vector2(collider->getCenter()), collider->getRadius(), 0.0f);

				if (rayIntersectsCollider) {
					List<GameObject*>::ListIterator it = outList->find(gameObject);

					// If not found then add it
					if (it.isNull()) {
						outList->pushBack(gameObject);
					}
				}
			}

		} else {
			FOR_ARRAY (i, mChildren) {
				Node* child = mChildren->get(i);

				if (child) {
					child->rayCastQuery(lineStart, lineEnd, outList);
				}
			}
		}
	}
}

QuadTree::QuadTree() : DE_Class() {
	mRoot = nullptr;
	mWidth = 0.0f;
	mHeight = 0.0f;
	mStatus = ColliderStatus::STATUS_NONE;
}

QuadTree::~QuadTree() {
	DE_TRACE()
	DE_FREE(mRoot);
}

void QuadTree::init(f32 size) {
	DE_TRACE()

	mWidth = size;
	mHeight = size;

	f32 minSize = EngineConfig::getInstance()->getF32("scene.quadTreeMinSize");

	mRoot = DE_NEW<Node>();
	mRoot->init(Vector2(-mWidth / 2.0f, mHeight / 2.0f), mWidth, mHeight, minSize, minSize, this);
}

void QuadTree::update() {
	DE_PROFILER_TIMEMARK_START()
	mRoot->update();
	DE_PROFILER_TIMEMARK_END()
}

void QuadTree::addCollider(Collider *collider) {
	collider->resetHasSizeChaged();
	mRoot->addCollider(collider);
}

void QuadTree::rayCastQuery(const Vector3 &lineStart, const Vector3 &lineEnd, List<GameObject*> *outList) {
	mRoot->rayCastQuery(lineStart, lineEnd, outList);
}

}
