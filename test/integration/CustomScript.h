#ifndef DE_CUSTOMSCRIPT_H
#define DE_CUSTOMSCRIPT_H

#include "Script.h"



namespace DE {

class Renderer;
class Transform;
class RigidBody;
class GameObject;

class CustomScript : public Script{
private:

  Renderer* mRenderer;
  RigidBody* mRigidBody;
  Transform* mTransform;

  GameObject* mTestObj;

public:

DE_CLASS(CustomScript, Script);

void createTestObj();
void createTestMap();

void init() override;
void step() override;
void terminate() override;
};

} /* namespace DE */

#endif /* DE_CUSTOMSCRIPT_H */
