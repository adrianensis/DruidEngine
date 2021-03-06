#include "TornadoScript.hpp"

#include "Core/TimeUtils.hpp"
#include "Log/Log.hpp"

#include "Scene/GameObject.hpp"
#include "UI/UI.hpp"
#include "Memory/Memory.hpp"
#include "Graphics/Renderer.hpp"
#include "Physics/RigidBody.hpp"
#include "Physics/Collider.hpp"
#include "Containers/List.hpp"
#include "Maths/Vector4.hpp"
#include "Input/Input.hpp"
#include "Graphics/Mesh.hpp"

#include "Graphics/Material.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/MaterialManager.hpp"
#include "Graphics/Animation/Animation.hpp"
#include "Scene/Transform.hpp"
#include "Scene/Scene.hpp"
#include "Graphics/Camera.hpp"

namespace DE {

TornadoScript::TornadoScript() : Script() {

}

TornadoScript::~TornadoScript() = default;

void TornadoScript::init() {

}

void TornadoScript::firstStep() {
	getGameObject()->setTag("tornado");
	mRenderer = getGameObject()->getFirstComponent<Renderer>();

	mDurationTime = 5.0f;
	mDurationTimeCounter = 0;
}

void TornadoScript::step() {
	if (mDurationTimeCounter >= mDurationTime) {
		ECHO("TORNADO DESTROY")
		getGameObject()->destroy();
	}
	mDurationTimeCounter += Time::getInstance()->getDeltaTimeSeconds();
}

void TornadoScript::onEnterCollision(GameObject *otherGameObject) {

}

void TornadoScript::onCollision(GameObject *otherGameObject) {

}

void TornadoScript::onExitCollision(GameObject *otherGameObject) {

}

void TornadoScript::terminate() {

}

}
