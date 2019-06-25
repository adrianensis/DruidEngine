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

	mIsDirtyTranslation = true;
	mIsDirtyRotation = true;
	mIsDirtyScale = true;
}

Transform::~Transform() {
	Memory::free<Matrix4>(mMatrix);
	Memory::free<Matrix4>(mTranslationMatrix);
	Memory::free<Matrix4>(mRotationMatrix);
	Memory::free<Matrix4>(mScaleMatrix);
}

void Transform::init(){
	mTranslationMatrix = Memory::allocate<Matrix4>();
	mTranslationMatrix->identity();

	mRotationMatrix = Memory::allocate<Matrix4>();
	mRotationMatrix->identity();

	mScaleMatrix = Memory::allocate<Matrix4>();
	mScaleMatrix->identity();

	mWorldPosition = Vector3(0.0f, 0.0f, 0.0f);
	mLocalPosition = Vector3(0.0f, 0.0f, 0.0f);
	mRotation = Vector3(0.0f, 0.0f, 0.0f);
	mScale = Vector3(1.0f, 1.0f, 1.0f);
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
	mIsDirtyTranslation = true;
	mWorldPosition = vector;
}

void Transform::setLocalPosition(const Vector3& vector){
	mIsDirtyTranslation = true;
	mLocalPosition = vector;
}

void Transform::setRotation(const Vector3& vector){
	mIsDirtyRotation = true;
	mRotation = vector;
};

void Transform::setScale(const Vector3& vector){
	mIsDirtyScale = true;
	mScale = vector;
};

void Transform::translate(const Vector3& vector){
	if(vector.len() > 0.0f){
		mIsDirtyTranslation = true;
		mLocalPosition.add(vector);
	}
};

void Transform::rotate(const Vector3& vector){
	if(vector.len() > 0.0f){
		mIsDirtyRotation = true;
		mRotation.add(vector);
	}
};

void Transform::lookAt(const Vector3& vector){
	if(vector.len() > 0.0f){
		mIsDirtyRotation = true;
	}
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
	if(mIsDirtyTranslation){
		mTranslationMatrix->translation(mLocalPosition);
		mIsDirtyTranslation = false;
	}

	return *mTranslationMatrix;
}

const Matrix4& Transform::getRotationMatrix() const{
	if(mIsDirtyRotation){
		mRotationMatrix->rotation(mRotation);
		mIsDirtyRotation = false;
	}

	return *mRotationMatrix;
}

const Matrix4& Transform::getScaleMatrix() const{
	if(mIsDirtyScale){
		mScaleMatrix->scale(mScale);
		mIsDirtyScale = false;
	}

	return *mScaleMatrix;
}


} /* namespace DE */
