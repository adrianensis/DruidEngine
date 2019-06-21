#include "Transform.h"
#include "GameObject.h"
#include "Matrix4.h"
#include "Memory.h"

namespace DE {

Transform::Transform() : Component()
{
	mMatrix = nullptr;
	mTranslationMatrix = nullptr;
	mRotationMatrix = nullptr;
	mScaleMatrix = nullptr;

	mIsDirty = false;
}

Transform::~Transform() {
	Memory::free<Matrix4>(mMatrix);
	Memory::free<Matrix4>(mTranslationMatrix);
	Memory::free<Matrix4>(mRotationMatrix);
	Memory::free<Matrix4>(mScaleMatrix);
}

const Vector3& Transform::getWorldPosition() const{
	return mWorldPosition;
}

const Vector3& Transform::getLocalPosition() const{
	return mLocalPosition;
}

const Vector3& Transform::getRotation() const{
	return mRotation;
};

const Vector3& Transform::getScale() const{
	return mScale;
};

void Transform::setWorldPosition(const Vector3& vector){
	mIsDirty = true;
	mWorldPosition = vector;
}

void Transform::setLocalPosition(const Vector3& vector){
	mIsDirty = true;
	mLocalPosition = vector;
}

void Transform::setRotation(const Vector3& vector){
	mIsDirty = true;
	mRotation = vector;
};

void Transform::setScale(const Vector3& vector){
	mIsDirty = true;
	mScale = vector;
};

void Transform::translate(const Vector3& vector){
	if(vector.len() > 0.0f){
		mIsDirty = true;
		mLocalPosition.add(vector);
	}
};

void Transform::rotate(const Vector3& vector){
	if(vector.len() > 0.0f){
		mIsDirty = true;
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

const Matrix4& Transform::getTranslationMatrix() const{
	if(!mTranslationMatrix){
		mTranslationMatrix = Memory::allocate<Matrix4>();
		mTranslationMatrix->identity();
	}

	mTranslationMatrix->translation(mLocalPosition);
	return *mTranslationMatrix;
}

const Matrix4& Transform::getRotationMatrix() const{
	return *mRotationMatrix;
}

const Matrix4& Transform::getScaleMatrix() const{
	return *mScaleMatrix;
}


} /* namespace DE */
