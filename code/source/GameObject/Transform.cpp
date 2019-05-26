#include "Transform.h"
#include "GameObject.h"
#include "Matrix4.h"

namespace DE {

Transform::Transform() : Component()
{
	// TODO Auto-generated constructor stub
}

Transform::~Transform() {
	// TODO Auto-generated destructor stub
}

const Vector3& Transform::getWorldPosition(){
	return mWorldPosition;
}

const Vector3& Transform::getLocalPosition(){
	return mLocalPosition;
}

const Vector3& Transform::getRotation(){
	return mRotation;
};

const Vector3& Transform::getScale(){
	return mScale;
};

void Transform::setWorldPosition(const Vector3& vector){
	mWorldPosition = vector;
}

void Transform::setLocalPosition(const Vector3& vector){
	mLocalPosition = vector;
}

void Transform::setRotation(const Vector3& vector){
	mRotation = vector;
};

void Transform::setScale(const Vector3& vector){
	mScale = vector;
};

void Transform::translate(const Vector3& vector){
	if(vector.len() > 0.0f){
		mLocalPosition.add(vector);
	}
};

void Transform::rotate(const Vector3& vector){
	if(vector.len() > 0.0f){
		mRotation.add(vector);
	}
};

void Transform::lookAt(const Vector3& vector){

	// this.dirty = true;
	//
	// this.target = vector.cpy();
	//
	// var pos = this.getPosition().cpy();
	//
	// this.forward = this.target.cpy().sub(pos).nor();
	//
	// var yAxis = new Vector3(0,1,0);
	// this.right = yAxis.cross(this.forward).nor();
	// this.up = this.forward.cpy().cross(this.right);

};

} /* namespace DE */
