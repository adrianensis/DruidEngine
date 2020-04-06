#ifndef DE_MAPEDITOR_H
#define DE_MAPEDITOR_H

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

class MapEditor : public Script{
private:

  class CellData : public DE_Class {
  public:
    DE_CLASS(CellData, DE_Class);

    Vector2 textureRegion;
    GameObject* tile = nullptr;
    Vector3 worldPosition;
  };

  u32 mGridSize;
  Array<Array<CellData*>*>* mGrid;

  Transform* mTransform;
  Camera* mCamera;
  Transform* mCameraTransform;

  GameObject* mBrush;
  GameObject* mTile;

  Texture* mTexture;
  Material* mMaterial;

  // f32 mTilesCount;
  Vector2 mAtlasSize;
  Vector2 mAtlasTextureSize;

  f32 mTileTextureSize;

  bool mTestCreated;
  u32 mTileIndex;
  u32 mTileSize;

  f32 mZoom;
  
public:

DE_CLASS(MapEditor, Script);

u32 mAtlasIndexX;
u32 mAtlasIndexY;

void createBrush();
void createAtlas();
void createTile(f32 x, f32 y);

void drawTile(CellData* cellData);
void removeTile(CellData* cellData);

void cameraMovement();

void init() override;
void firstStep() override;
void step() override;
void terminate() override;
};

} /* namespace DE */

#endif /* DE_MAPEDITOR_H */
