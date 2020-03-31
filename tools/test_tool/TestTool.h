#ifndef DE_TESTTOOL_H
#define DE_TESTTOOL_H

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

  class CellData {
  public:
    Vector2 textureRegion;
    bool isSet = false;
    UIButton* button = nullptr;
  };

  u32 mGridSize;
  Array<Array<CellData*>*>* mGrid;

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
  u32 mTileSize;

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

#endif /* DE_TESTTOOL_H */
