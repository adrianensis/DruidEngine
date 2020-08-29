#include "TornadoScript.hpp"

#include <Time.hpp>
#include "Log.hpp"

#include "GameObject.hpp"
#include "UI.hpp"
#include "Memory.hpp"
#include "Renderer.hpp"
#include "RigidBody.hpp"
#include "Collider.hpp"
#include "List.hpp"
#include "Vector4.hpp"
#include "Input.hpp"
#include "Mesh.hpp"

#include "Material.hpp"
#include "Texture.hpp"
#include "MaterialManager.hpp"
#include "Animation.hpp"
#include "Transform.hpp"
#include "Scene.hpp"
#include "Camera.hpp"

namespace DE {

// ---------------------------------------------------------------------------

TornadoScript::TornadoScript() : Script() {

}

// ---------------------------------------------------------------------------

TornadoScript::~TornadoScript() = default;

void TornadoScript::init() {

}

// ---------------------------------------------------------------------------

void TornadoScript::firstStep() {
	getGameObject()->setTag("tornado");
	mRenderer = getGameObject()->getComponents<Renderer>()->get(0);

	mDurationTime = 5.0f;
	mDurationTimeCounter = 0;
}

// ---------------------------------------------------------------------------

void TornadoScript::step() {
	if (mDurationTimeCounter >= mDurationTime) {
		ECHO("TORNADO DESTROY")
		getGameObject()->destroy();
	}
	mDurationTimeCounter += Time::getInstance()->getDeltaTimeSeconds();
}

// ---------------------------------------------------------------------------

void TornadoScript::onEnterCollision(GameObject *otherGameObject) {

}

void TornadoScript::onCollision(GameObject *otherGameObject) {

}

void TornadoScript::onExitCollision(GameObject *otherGameObject) {

}

void TornadoScript::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
