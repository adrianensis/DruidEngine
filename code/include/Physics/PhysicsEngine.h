#ifndef DE_PHYSICSENGINE_H
#define DE_PHYSICSENGINE_H

#include "DE_Class.h"
#include "Singleton.h"

namespace DE {

class RigidBody;
class QuadTree;
template<class T> class List;
class Iterator;

class PhysicsEngine : public DE_Class, public Singleton<PhysicsEngine>{
private:

  List<RigidBody*>* mRigidBodies;
  QuadTree* mQuadTree;

  void internalRemoveRigidBody(const Iterator* it);

public:

  DE_CLASS(PhysicsEngine, DE_Class);

  void addRigidBody(RigidBody* rigidBody);

  void init();
  void step(f32 deltaTime);
  void terminate();

};
} /* namespace DE */
#endif /* DE_PHYSICSENGINE_H */
