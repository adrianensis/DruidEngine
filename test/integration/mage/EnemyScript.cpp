#include "EnemyScript.hpp"

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
#include "ProjectileScript.hpp"
#include "TornadoScript.hpp"
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
	mRenderer = getGameObject()->getComponents<Renderer>()->get(0);
	mRigidBody = getGameObject()->getComponents<RigidBody>()->get(0);

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

	if (mRenderer->isOutOfCamera() && (getGameObject()->getTransform()->getWorldPosition().x < 0 || mFalling)) {
		ECHO("DESTROY ENEMY OUT OF CAMERA")
		getGameObject()->destroy();
	}
}

// ---------------------------------------------------------------------------

void EnemyScript::onEnterCollision(GameObject *otherGameObject) {

	if (!mFalling && !otherGameObject->isPendingToBeDestroyed() && !otherGameObject->isDestroyed()) {

		if (otherGameObject->getTag() == "projectile") {
			ProjectileScript* projectileScript = (ProjectileScript*) otherGameObject->getComponents<Script>()->get(0);

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

	if (!mFalling && !otherGameObject->isPendingToBeDestroyed() && !otherGameObject->isDestroyed()
			&& otherGameObject->getTag() == "tornado") {
		TornadoScript* projectileScript = (TornadoScript*) otherGameObject->getComponents<Script>()->get(0);

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
