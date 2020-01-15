#ifndef DE_CUSTOMSCRIPT_H
#define DE_CUSTOMSCRIPT_H

#include "Script.h"
#include "Vector2.h"

namespace DE {

class Renderer;
class Transform;
class RigidBody;
class Collider;
class GameObject;
class Camera;
class UIButton;
class Texture;
class Material;
template <class K, class V> class HashMap;

class TestTool : public Script{
private:

  Transform* mTransform;
  Camera* mCamera;

  GameObject* mTestObj;
  UIButton* mTestButton;

  Texture* mTexture;
  Material* mMaterial;

  bool mTestCreated;
  u32 mTileIndex;

public:

DE_CLASS(TestTool, Script);

void createTestObj();
void createTestButton();
void createTile();
void createFont();

void init() override;
void step() override;
void terminate() override;
};

} /* namespace DE */

#endif /* DE_CUSTOMSCRIPT_H */
