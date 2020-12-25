#ifndef DE_GameController_H
#define DE_GameController_H

#include "Script.hpp"
#include "Vector2.hpp"
#include "Vector4.hpp"
#include "PlayerScript.hpp"
#include "Element.hpp"

namespace DE {

class Renderer;
class Transform;
class RigidBody;
class Collider;
class GameObject;
class Camera;
class UIButton;
class UIText;
class Material;
template<class T> class Array;
template<class T> class List;
template<class K, class V> class HashMap;

class GameController: public Script {
private:

	GameObject* mPlayer;
	PlayerScript* mPlayerScript;
	GameObject* mCrossHair;
	// Array<GameObject*>* mRenderFloor;
	List<GameObject*>* mRenderFloor;
	u32 mRenderTilesAmount;
	List<GameObject*>* mRecycledTiles;
	Array<GameObject*>* mPhysicFloor;
	GameObject* mBackground;
	f32 mRenderTileSize;

	Array<UIButton*>* mBookSelector;
	UIButton* mSelectedBook;

	UIText* mTextMana;

	f32 mEnemySpawnTimeCount;
	f32 mEnemySpawnTime;

public:
	static f32 smGlobalSpeed;

	DE_CLASS(GameController, Script)

	void createPlayer(f32 x, f32 y);
	void createCrossHair();
	void createEnemy(f32 x, f32 y);
	void createProjectile(f32 x, f32 y, f32 clickX, f32 clickY);
	GameObject* createRenderFloorTile(f32 x, f32 y);
	GameObject* createPhysicFloorTile(f32 x, f32 y);
	void createBackground(f32 x, f32 y);
	UIButton* createBook(f32 x, f32 y, const Vector4 &color, Element element, f32 size);

	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;

};

} /* namespace DE */

#endif /* DE_GameController_H */
