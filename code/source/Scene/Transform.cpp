#include "Transform.h"
#include "GameObject.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Memory.h"
#include "Log.h"
#include "RenderEngine.h"

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

	mParent = nullptr;
}

Transform::~Transform() {
	Memory::free<Matrix4>(mModelMatrix);
	Memory::free<Matrix4>(mTranslationMatrix);
	Memory::free<Matrix4>(mRotationMatrix);
	Memory::free<Matrix4>(mScaleMatrix);
}

// ---------------------------------------------------------------------------

void Transform::init(){
	TRACE();

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

bool Transform::isDirtyTranslation() const { return mIsDirtyTranslation; }
void Transform::setDirtyTranslation(bool dirty) { mIsDirtyTranslation = dirty; }


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

	// RenderEngine::getInstance()->drawLine(mLocalPosition, Vector3(mLocalPosition.x + 400.0f ,mLocalPosition.y,mLocalPosition.z));
  // RenderEngine::getInstance()->drawLine(mLocalPosition, Vector3(mLocalPosition.x,mLocalPosition.y + 400.0f ,mLocalPosition.z));
  // RenderEngine::getInstance()->drawLine(mLocalPosition, Vector3(mLocalPosition.x,mLocalPosition.y,mLocalPosition.z + 400.0f ));

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

void Transform::setParent(Transform* parent){ mParent = parent; }

Transform* Transform::getParent(){ return mParent; }

// ---------------------------------------------------------------------------

} /* namespace DE */
