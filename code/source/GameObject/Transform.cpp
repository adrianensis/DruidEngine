#include "Transform.h"
#include "GameObject.h"

namespace DE {

Transform::Transform() : Component()
{
	// TODO Auto-generated constructor stub
}

Transform::~Transform() {
	// TODO Auto-generated destructor stub
}

Vector3& Transform::getWorldPosition(){
	return mWorldPosition;
}

Vector3& Transform::getLocalPosition(){
	return mLocalPosition;
}

void Transform::setWorldPosition(const Vector3& newVector){
	mWorldPosition = newVector;
}

void Transform::setLocalPosition(const Vector3& newVector){
	mLocalPosition = newVector;
}



} /* namespace DE */
