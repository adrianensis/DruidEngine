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

u32 ProjectileScript::smProjectileIndex = 0;

ProjectileScript::ProjectileScript() : Script() {

}

ProjectileScript::~ProjectileScript() = default;

void ProjectileScript::init() {

}

void ProjectileScript::firstStep() {
	getGameObject()->setTag("projectile" + std::to_string(smProjectileIndex));
	++smProjectileIndex;
	mRenderer = getGameObject()->getFirstComponent<Renderer>();

	mExplosionTime = 0.1f;
	mExplosionTimeCounter = 0;
	mIsExploding = false;
}

void ProjectileScript::step() {
	RigidBody* rigidBody = getGameObject()->getFirstComponent<RigidBody>();

	//rigidBody->setLinear(rigidBody->getLinear() - (rigidBody->getLinear().nor() * 3));

	if(getGameObject()->isActive()){
		if (mIsExploding) {
			if (mExplosionTimeCounter >= mExplosionTime) {
				ECHO("PROJECTILE DESTROY - EXPLOSION")
				getGameObject()->getScene()->removeGameObject(getGameObject());
			}
			mExplosionTimeCounter += Time::getInstance()->getDeltaTimeSeconds();
		}

		if(mRenderer->getIsOutOfCamera()){
			ECHO("PROJECTILE DESTROY - OUT OF CAMERA")
			getGameObject()->getScene()->removeGameObject(getGameObject());
		}
	}
}

void ProjectileScript::explode() {
	mIsExploding = true;
	mRenderer->setAnimation("explosion");
	getGameObject()->getFirstComponent<RigidBody>()->setLinear(Vector3(0, 0, 0));
}

void ProjectileScript::onEnterCollision(GameObject *otherGameObject) {
	/*ECHO("PROJECTILE ON ENTER COLLISION")
	RigidBody* rigidBody = getGameObject()->getComponents<RigidBody>()->get(0);

	if(otherGameObject->getTag() == "projectile"){
		Vector3 dst = Vector3(getGameObject()->getTransform()->getWorldPosition()).sub(otherGameObject->getTransform()->getWorldPosition());
		f32 len = dst.len();
		rigidBody->setLinear(dst.nor() * len * 2);
	} else {
		rigidBody->setLinear(rigidBody->getLinear() / 2.0f * -1);
	}*/

	getGameObject()->getScene()->removeGameObject(getGameObject());
}

void ProjectileScript::onCollision(GameObject *otherGameObject) {

	/*ECHO("PROJECTILE ON COLLISION")
		Vector3 dst = Vector3(getGameObject()->getTransform()->getWorldPosition()).sub(otherGameObject->getTransform()->getWorldPosition());
		f32 len = dst.len();
		RigidBody* rigidBody = getGameObject()->getComponents<RigidBody>()->get(0);
		Vector3 speed = (dst.nor() + Vector3(1,1,0))* 100;

		rigidBody->setLinear(rigidBody->getLinear() + speed);*/
}

void ProjectileScript::onExitCollision(GameObject *otherGameObject) {

}

void ProjectileScript::terminate() {

}

}
