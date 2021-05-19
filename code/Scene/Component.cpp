#include "Scene/Component.hpp"
#include "Config/ConfigMap.hpp"
#include "Scene/GameObject.hpp"

Component::Component() {
	mIsActive = true;
	mIsDestroyed = false;
	mIsStatic = false;
}

bool Component::isStatic() {

	if(mGameObject){
		mIsStatic = mGameObject->getIsStatic();
	}

	return mIsStatic;
}

void Component::destroy() {
	if (!(getIsDestroyed() || getIsPendingToBeDestroyed())) {
		mIsPendingToBeDestroyed = true;
		mIsActive = false;
		onDestroy();
	}
};

void Component::save(ConfigMap* configMap, std::string& objectName) {
	
}

void Component::load(ConfigMap* configMap, std::string& objectName) {

}