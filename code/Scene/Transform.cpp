#include "Scene/Transform.hpp"
#include "Scene/GameObject.hpp"
#include "Maths/Quaternion.hpp"
#include "Log/Log.hpp"

Transform::Transform()
{
	mIsDirtyTranslation = true;
	mIsDirtyRotation = true;
	mIsDirtyScale = true;

	mParent = nullptr;

	mAffectedByProjection = true;
	mModelMatrixGenerated = false;
	mForceModelMatrixCalculation = false;
}

Transform::~Transform()
{
	mParent = nullptr;
}

void Transform::init()
{
	// TRACE();
	mTranslationMatrix.identity();
	mRotationMatrix.identity();
	mScaleMatrix.identity();

	mWorldPosition = Vector3(0.0f, 0.0f, 0.0f);
	mLocalPosition = Vector3(0.0f, 0.0f, 0.0f);
	mRotation = Vector3(0.0f, 0.0f, 0.0f);
	mScale = Vector3(1.0f, 1.0f, 1.0f);

	mModelMatrixGenerated = false;
	mForceModelMatrixCalculation = false;
}

bool Transform::isDirtyTranslation() CNS
{
	return mIsDirtyTranslation || (mParent && mParent->isDirtyTranslation());
}

void Transform::setDirtyTranslation(bool dirty)
{
	mIsDirtyTranslation = dirty;
}

void Transform::setLocalPosition(CNS Vector3 &vector)
{
	mIsDirtyTranslation = true;
	mLocalPosition = vector;
}

void Transform::setRotation(CNS Vector3 &vector)
{
	mIsDirtyRotation = true;
	mRotation = vector;
}

void Transform::setScale(CNS Vector3 &vector)
{
	mIsDirtyScale = true;
	mScale = vector;
}

Vector3 Transform::calculateWorldPosition() CNS
{
	Vector3 worldPosition = mLocalPosition;

	if (mParent)
	{
		worldPosition.add(mParent->getWorldPosition());

		Matrix4 rotationMatrix;
		rotationMatrix.init(mParent->getRotationMatrix());

		worldPosition = Vector3(rotationMatrix.mulVector(Vector4(worldPosition, 1.0f)));
	}

	return worldPosition;
}

CNS Vector3 &Transform::getWorldPosition()
{
	mWorldPosition = calculateWorldPosition();
	return mWorldPosition;
}

void Transform::translate(CNS Vector3 &vector)
{
	if (vector.len() > 0.0f)
	{
		mIsDirtyTranslation = true;
		mLocalPosition.add(vector);
	}
}

void Transform::rotate(CNS Vector3 &vector)
{
	if (vector.len() > 0.0f)
	{
		mIsDirtyRotation = true;
		mRotation.add(vector);
	}
}

void Transform::lookAt(CNS Vector3 &targetPosition)
{
	/*mIsDirtyRotation = true;

	Vector3 target(targetPosition);

	Vector3 forward(target.sub(mLocalPosition).nor());

	Vector3 yAxis(0, 1, 0);
	Vector3 right(yAxis.cross(forward).nor());
	Vector3 up(Vector3(forward).cross(right));

	Matrix4 lookAtMatrix;
	lookAtMatrix.init(Vector4(right.x, right.y, right.z, 0), Vector4(up.x, up.y, up.z, 0),
			Vector4(forward.x, forward.y, forward.z, 0), Vector4(0, 0, 0, 1));

	Quaternion q;

	q.fromMatrix(lookAtMatrix);

	mRotation = q.toEuler();*/
}

CNS Matrix4 &Transform::getTranslationMatrix()
{
	if (mIsDirtyTranslation || mParent)
	{
		mTranslationMatrix.translation(getWorldPosition());
		mIsDirtyTranslation = false;
	}

	// RenderEngine::getInstance()->drawLine(mLocalPosition, Vector3(mLocalPosition.x + 400.0f ,mLocalPosition.y,mLocalPosition.z));
	// RenderEngine::getInstance()->drawLine(mLocalPosition, Vector3(mLocalPosition.x,mLocalPosition.y + 400.0f ,mLocalPosition.z));
	// RenderEngine::getInstance()->drawLine(mLocalPosition, Vector3(mLocalPosition.x,mLocalPosition.y,mLocalPosition.z + 400.0f ));

	return mTranslationMatrix;
}

CNS Matrix4 &Transform::getRotationMatrix()
{
	if (mIsDirtyRotation)
	{
		mRotationMatrix.rotation(mRotation);
		mIsDirtyRotation = false;
	}

	return mRotationMatrix;
}

CNS Matrix4 &Transform::getScaleMatrix()
{
	if (mIsDirtyScale)
	{
		mScaleMatrix.scale(mScale);
		mIsDirtyScale = false;
	}

	return mScaleMatrix;
}

CNS Matrix4 &Transform::getModelMatrix(bool force /*= false*/)
{
	if (!isStatic() || (isStatic() && !mModelMatrixGenerated) || force || mForceModelMatrixCalculation)
	{
		mModelMatrix.init(getTranslationMatrix());
		Matrix4 rotationMatrix(getRotationMatrix());
		Matrix4 scaleMatrix(getScaleMatrix());

		scaleMatrix.mul(rotationMatrix);
		mModelMatrix.mul(scaleMatrix);

		mModelMatrixGenerated = true;
		mForceModelMatrixCalculation = false;
	}

	return mModelMatrix;
}

SERIALIZE_IMPL(Transform)
{
	SUPER_SERIALIZE()

	DO_SERIALIZE("local_position", mLocalPosition);
	DO_SERIALIZE("scale", mScale);
	DO_SERIALIZE("rotation", mRotation);
}

DESERIALIZE_IMPL(Transform)
{
	SUPER_DESERIALIZE()

	DO_DESERIALIZE("local_position", mLocalPosition);
	DO_DESERIALIZE("scale", mScale);
	DO_DESERIALIZE("rotation", mRotation);

	mIsDirtyTranslation = true;
	mIsDirtyRotation = true;
	mIsDirtyScale = true;
}