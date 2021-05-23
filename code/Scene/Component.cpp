#include "Scene/Component.hpp"
#include "Config/ConfigObject.hpp"
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

void Component::save(ConfigObject* configMap, std::string& objectName) {
	
}

void Component::load(ConfigObject* configMap, std::string& objectName) {

}