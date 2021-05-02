#include "Scene/Component.hpp"
#include "Config/ConfigMap.hpp"
#include "Scene/GameObject.hpp"

namespace DE {

Component::Component() : ObjectBase() {
	mGameObject = nullptr;
	mIsActive = true;
	mIsDestroyed = false;
	mIsStatic = false;
}

Component::~Component() = default;

bool Component::isStatic() {

	if(mGameObject){
		mIsStatic = mGameObject->getIsStatic();
	}

	return mIsStatic;
}

void Component::save(ConfigMap* configMap, StringRef objectName) {
	
}

void Component::load(ConfigMap* configMap, StringRef objectName) {

}

}
