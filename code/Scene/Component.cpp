#include "Scene/Component.hpp"
#include "Config/ConfigMap.hpp"
#include "Scene/GameObject.hpp"

namespace DE {

Component::Component() : DE_Class() {
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

void Component::save(ConfigMap* configMap, const std::string& objectName) {
	
}

void Component::load(ConfigMap* configMap, const std::string& objectName) {

}

}
