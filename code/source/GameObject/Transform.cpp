#include "Transform.h"
#include "GameObject.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Memory.h"
#include "Debug.h"

namespace DE {

// ---------------------------------------------------------------------------

Transform::Transform() : Component()
{
	mModelMatrix = nullptr;
	mTranslationMatrix = nullptr;
	mRotationMatrix = nullptr;
	mScaleMatrix = nullptr;

	mIsDirtyTranslation = true;
	mIsDirtyRotation = true;
	mIsDirtyScale = true;
}

Transform::~Transform() {
	Memory::free<Matrix4>(mModelMatrix);
	Memory::free<Matrix4>(mTranslationMatrix);
	Memory::free<Matrix4>(mRotationMatrix);
	Memory::free<Matrix4>(mScaleMatrix);
}

// ---------------------------------------------------------------------------

void Transform::init(){
	// mModelMatrix = Memory::allocate<Matrix4>();
	// mModelMatrix->identity();

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

const Vector3& Transform::getWorldPosition() const{ return mWorldPosition; }
const Vector3& Transform::getLocalPosition() const{ return mLocalPosition; }
const Vector3& Transform::getRotation() const{ return mRotation; }
const Vector3& Transform::getScale() const{ return mScale; }

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

// ---------------------------------------------------------------------------

void Transform::translate(const Vector3& vector){
	if(vector.len() > 0.0f){
		mIsDirtyTranslation = true;
		mLocalPosition.add(vector);
	}
};

// ---------------------------------------------------------------------------

void Transform::rotate(const Vector3& vector){
	if(vector.len() > 0.0f){
		mIsDirtyRotation = true;
		mRotation.add(vector);
	}
};

// ---------------------------------------------------------------------------

void Transform::lookAt(const Vector3& targetPosition) {

	mIsDirtyRotation = true;

	Vector3 target(targetPosition);

	Vector3 forward(target.sub(mLocalPosition).nor());

	Vector3 yAxis(0,1,0);
	Vector3 right(yAxis.cross(forward).nor());
	Vector3 up(Vector3(forward).cross(right));

	Matrix4 lookAtMatrix;
	lookAtMatrix.init(
		Vector4(right.x, right.y, right.z, 0),
		Vector4(up.x, up.y, up.z, 0),
		Vector4(forward.x, forward.y, forward.z, 0),
		Vector4(0, 0, 0, 1)
	);

	Quaternion q;

	q.fromMatrix(lookAtMatrix);

	mRotation = q.toEuler();
}

// ---------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------

} /* namespace DE */
