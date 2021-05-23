#include "ProjectileScript.hpp"

#include "Core/Time/TimeUtils.hpp"
#include "Log/Log.hpp"

#include "Scene/GameObject.hpp"
#include "Graphics/Renderer.hpp"
#include "Maths/Vector4.hpp"
//#include "Input/Input.hpp"
#include "Graphics/Mesh.hpp"

#include "Graphics/Material.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/MaterialManager.hpp"
#include "Graphics/Animation/Animation.hpp"
#include "Scene/Transform.hpp"
#include "Scene/Scene.hpp"
#include "Graphics/Camera.hpp"

u32 ProjectileScript::smProjectileIndex = 0;

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

}

void ProjectileScript::terminate() {

}