#include "ProjectileScript.hpp"

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
	mRenderer = getGameObject()->getFirstComponent<Renderer>();

	mExplosionTime = 0.3f;
	mExplosionTimeCounter = 0;
	mIsExploding = false;
}

// ---------------------------------------------------------------------------

void ProjectileScript::step() {
	// if(mRenderer->getIsOutOfCamera()){
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
	getGameObject()->getFirstComponent<RigidBody>()->setLinear(Vector3(0, 0, 0));
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
