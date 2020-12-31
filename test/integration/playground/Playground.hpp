#ifndef DE_Playground_H
#define DE_Playground_H

#include "Scripting/Script.hpp"
#include "Maths/Vector2.hpp"
#include <string>

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

class Playground: public Script {
private:

	Transform* mTransform;
	Camera* mCamera;
	Transform* mCameraTransform;


	bool mCameraControl;


public:

	GameObject* mPlayer;
	Material* mMaterial;

	DE_CLASS(Playground, Script)

	GameObject* createProjectile(f32 x, f32 y);
	void createPlayer();
	void processMovement();

	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;
};

} /* namespace DE */

#endif /* DE_Playground_H */
