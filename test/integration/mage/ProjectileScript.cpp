#include "ProjectileScript.hpp"
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
#include "Time2.hpp"

namespace DE {

// ---------------------------------------------------------------------------

ProjectileScript::ProjectileScript() : Script() {

}

// ---------------------------------------------------------------------------

ProjectileScript::~ProjectileScript() = default;

void ProjectileScript::init() {

}

// ---------------------------------------------------------------------------

void ProjectileScript::firstStep() {
	getGameObject()->setTag("projectile");
	mRenderer = getGameObject()->getComponents<Renderer>()->get(0);

	mExplosionTime = 0.3f;
	mExplosionTimeCounter = 0;
	mIsExploding = false;
}

// ---------------------------------------------------------------------------

void ProjectileScript::step() {
	// if(mRenderer->isOutOfCamera()){
	//   ECHO("PROJECTILE DESTROY")
	//   getGameObject()->destroy();
	// }

	if (mIsExploding) {
		if (mExplosionTimeCounter >= mExplosionTime) {
			ECHO("PROJECTILE DESTROY")
			getGameObject()->destroy();
		}
		mExplosionTimeCounter += Time::getInstance()->getDeltaTimeSeconds();
	}
}

// ---------------------------------------------------------------------------

void ProjectileScript::explode() {
	mIsExploding = true;
	mRenderer->setAnimation("explosion");
	getGameObject()->getComponents<RigidBody>()->get(0)->setLinear(Vector3(0, 0, 0));
}

// ---------------------------------------------------------------------------

void ProjectileScript::onEnterCollision(GameObject *otherGameObject) {

}

void ProjectileScript::onCollision(GameObject *otherGameObject) {

}

void ProjectileScript::onExitCollision(GameObject *otherGameObject) {

}

void ProjectileScript::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
