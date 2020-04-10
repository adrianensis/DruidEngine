#ifndef DE_CUSTOMSCRIPT_H
#define DE_CUSTOMSCRIPT_H

#include "Script.h"
#include "Vector2.h"
#include "PlayerScript.h"

namespace DE {

class Renderer;
class Transform;
class RigidBody;
class Collider;
class GameObject;
class Camera;
class UIButton;
class Material;
template <class K, class V> class HashMap;

class CustomScript : public Script{
private:

  Renderer* mRenderer;
  Renderer* mRendererMinion;
  RigidBody* mRigidBody;
  Collider* mCollider;
  Transform* mTransform;
  Camera* mCamera;

  GameObject* mTestObj;
  GameObject* mTestMinion;
  UIButton* mTestButton;

  bool mTestCreated;

  Renderer* mRendererMountain;
  Renderer* mRendererMountain2;
  Renderer* mRendererForest;
  Renderer* mRendererForest2;

public:

DE_CLASS(CustomScript, Script);

void createTestObj();
void createTestButton();
void createTestMap();
void createTestScene();
void createTestBackground(float x, float y);
void createTestTile(float x, float y, Material* material);
void createFont();

void init() override;
void firstStep() override;
void step() override;
void terminate() override;

};

} /* namespace DE */

#endif /* DE_CUSTOMSCRIPT_H */
