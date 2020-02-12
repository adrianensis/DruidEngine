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
template <class T> class Array;

class TestTool : public Script{
private:

  u32 mGridSize;
  Array<Array<bool>*>* mGrid;

  Transform* mTransform;
  Camera* mCamera;
  Transform* mCameraTransform;

  GameObject* mBrush;
  GameObject* mTestTile;
  UIButton* mTestButton;

  Texture* mTexture;
  Material* mMaterial;

  bool mTestCreated;
  u32 mTileIndex;

public:

DE_CLASS(TestTool, Script);

u32 mAtlasIndexX;
u32 mAtlasIndexY;

void createBrush();
void createTestButton();
void createAtlas();
void createFont();
void createTile(f32 x, f32 y);

void init() override;
void step() override;
void terminate() override;
};

} /* namespace DE */

#endif /* DE_CUSTOMSCRIPT_H */
