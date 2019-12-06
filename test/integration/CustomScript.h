#ifndef DE_CUSTOMSCRIPT_H
#define DE_CUSTOMSCRIPT_H

#include "Script.h"

namespace DE {

class Renderer;
class Transform;
class RigidBody;
class Collider;
class GameObject;
class Camera;

class CustomScript : public Script{
private:

  Renderer* mRenderer;
  RigidBody* mRigidBody;
  Collider* mCollider;
  Transform* mTransform;
  Camera* mCamera;

  u32 counter = 0;

  GameObject* mTestObj;
  GameObject* mTestButton;

  bool mTestCreated;

public:

DE_CLASS(CustomScript, Script);

void createTestObj();
void createTestButton();
void createTestMap();

void init() override;
void step() override;
void terminate() override;
};

} /* namespace DE */

#endif /* DE_CUSTOMSCRIPT_H */
