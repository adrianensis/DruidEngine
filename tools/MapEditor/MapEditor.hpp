#pragma once

#include "Scripting/Script.hpp"
#include <string>

#include "Events/Event.hpp"
#include "MapEditorUI/Grid/Grid.hpp"
#include "MapEditorUI/MapEditorUI.hpp"

namespace DE {

class MapEditor : public Script {
private:

	 Transform* mTransform;
	 Transform* mCameraTransform;
	 bool mCameraControl;

	 bool mIsMapLoaded;

public:

	 Camera* mCamera;
	 u32 mLayer;
	 f32 mZoom;
	 GameObject* mPlayer;
	 MapEditorUI mMapEditorUI;

	 Grid mGrid;
	 ConfigMap* mConfigMap;

	GENERATE_METADATA(MapEditor);

	MapEditor();
	virtual ~MapEditor() override;

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
}

