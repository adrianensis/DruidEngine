#ifndef DE_TESTUI_H
#define DE_TESTUI_H

#include "Script.h"
#include "Vector2.h"
#include "UI.h"

namespace DE {

class Renderer;
class Transform;
class RigidBody;
class Collider;
class GameObject;
class Camera;
class UIButton;
class UIText;
class Texture;
class Material;
template <class K, class V> class HashMap;
template <class T> class Array;

class TestUI : public Script{
private:

  Camera* mCamera;
  Transform* mCameraTransform;

  UIButton* mTestButton;
  UIText* mTestText;
  UIList* mTestList;

  Texture* mTexture;
  Material* mMaterial;

public:

DE_CLASS(TestUI, Script);

void init() override;
void step() override;
void terminate() override;
};

} /* namespace DE */

#endif /* DE_TESTUI_H */
