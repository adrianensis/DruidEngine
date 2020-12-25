#ifndef DE_MAPEDITOR_H
#define DE_MAPEDITOR_H

#include "Script.hpp"
#include "Vector2.hpp"
#include "MapEditorUI.hpp"
#include "Grid.hpp"
#include <string>

#include "Event.hpp"

namespace DE {

class Transform;
class Renderer;
class RigidBody;
class Collider;
class GameObject;
class Camera;
class UIButton;
class UIText;
class Texture;
class Material;
template<class K, class V> class HashMap;
template<class T> class Array;
template<class T> class List;
class ConfigMap;



class MapEditor: public Script {
private:

	Transform* mTransform = nullptr;
	Camera* mCamera = nullptr;
	Transform* mCameraTransform = nullptr;

	bool mCameraControl = true;

public:

	u32 mLayer = 0;
	f32 mZoom = 1;
	GameObject* mPlayer = nullptr;
	MapEditorUI mMapEditorUI;
	Material* mMaterial = nullptr;

	Grid mGrid;
	ConfigMap* mConfigMap = nullptr;

	DE_CLASS(MapEditor, Script)

	void createPlayer();
	void destroyPlayer();

	void addColliderToTile(GameObject* tile);

	void cameraZoom();
	void processMovement();
	void switchCameraControl();

	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;
};

} /* namespace DE */

#endif /* DE_MAPEDITOR_H */
