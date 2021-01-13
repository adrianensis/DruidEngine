#include "EnemyScript.hpp"

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
#include "Graphics/Animation.hpp"
#include "Scene/Transform.hpp"
#include "Scene/Scene.hpp"
#include "Graphics/Camera.hpp"
#include "Spells/ProjectileScript.hpp"
#include "Spells/TornadoScript.hpp"
#include "GameController.hpp"

namespace DE {

// ---------------------------------------------------------------------------

EnemyScript::EnemyScript() : Script() {

}

// ---------------------------------------------------------------------------

EnemyScript::~EnemyScript() = default;

void EnemyScript::init() {

}

// ---------------------------------------------------------------------------

void EnemyScript::firstStep() {

	getGameObject()->setTag("enemy");
	mRenderer = getGameObject()->getFirstComponent<Renderer>();
	mRigidBody = getGameObject()->getFirstComponent<RigidBody>();

	mSlowDown = false;
	mSpeed = GameController::smGlobalSpeed;
}

// ---------------------------------------------------------------------------

void EnemyScript::step() {
	if (mFalling) {
		mRigidBody->setLinear(Vector3(0, -600, 0));
	} else {
		mRigidBody->setLinear(Vector3(-mSpeed, 0, 0));
		mSpeed = GameController::smGlobalSpeed;
	}

	if (mRenderer->getIsOutOfCamera() && (getGameObject()->getTransform()->getWorldPosition().x < 0 || mFalling)) {
		ECHO("DESTROY ENEMY OUT OF CAMERA")
		getGameObject()->destroy();
	}
}

// ---------------------------------------------------------------------------

void EnemyScript::onEnterCollision(GameObject *otherGameObject) {

	if (!mFalling && !otherGameObject->getIsPendingToBeDestroyed() && !otherGameObject->getIsDestroyed()) {

		if (otherGameObject->getTag() == "projectile") {
			ProjectileScript* projectileScript = (ProjectileScript*) otherGameObject->getFirstComponent<Script>();

			if (!projectileScript->isExploded()) {

				Element projectileElement = projectileScript->getElement();
				if ((mElement == Element::FIRE && projectileElement == Element::ICE)
						|| (mElement == Element::ICE && projectileElement == Element::FIRE)) {
					// ECHO("DESTROY ENEMY")
					// getGameObject()->destroy();
					mFalling = true;
					mRenderer->setAnimation("death");
					projectileScript->explode();
				}
			}
		}

	}
}

void EnemyScript::onCollision(GameObject *otherGameObject) {

	if (!mFalling && !otherGameObject->getIsPendingToBeDestroyed() && !otherGameObject->getIsDestroyed()
			&& otherGameObject->getTag() == "tornado") {
		TornadoScript* projectileScript = (TornadoScript*) otherGameObject->getFirstComponent<Script>();

		mSpeed = GameController::smGlobalSpeed / 4.0f;
	}
}

void EnemyScript::onExitCollision(GameObject *otherGameObject) {
	std::string x = "onExitCollision " + getGameObject()->getTag();
	ECHO(x);

	// mSpeed = GameController::smGlobalSpeed;

}

// ---------------------------------------------------------------------------

void EnemyScript::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
