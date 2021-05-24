#pragma once

#include "Scripting/Script.hpp"
#include "Maths/Vector2.hpp"
#include <string>

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

CLASS(Playground, Script) {

	PRI(Transform, NONE, Transform*)

	PUB(Player, NONE, GameObject*)
	PUB(Material, NONE, Material*)

public:

	void createPlayer(const Vector2& position);
	void processMovement();

	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;
};