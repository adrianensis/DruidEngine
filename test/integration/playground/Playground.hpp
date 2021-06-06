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

	PUB(Material, NONE, Material*)

public:

	void createSprite(const Vector2& position, u32 type = 0);

	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;
};