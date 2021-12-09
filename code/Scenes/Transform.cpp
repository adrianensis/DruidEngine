#include "Scenes/Transform.hpp"
#include "Scenes/GameObject.hpp"

TransformState::TransformState(const Transform& transform)
{
	mWorldPosition = transform.getWorldPosition();
	mRotation = transform.getRotation();
	mScale = transform.getScale();
}

Transform::Transform()
{
	mParent = nullptr;

	mAffectedByProjection = true;
	mModelMatrixGenerated = false;
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
}

void Transform::setLocalPosition(const Vector3 &vector)
{
	mLocalPosition = vector;
}

void Transform::setRotation(const Vector3 &vector)
{
	mRotation = vector;
}

void Transform::setScale(const Vector3 &vector)
{
	mScale = vector;
}

const Vector3 &Transform::getWorldPosition() const
{
	mWorldPosition = mLocalPosition;

	if (mParent)
	{
		mWorldPosition = mParent->getModelMatrix().mulVector(Vector4(mWorldPosition, 1));
	}

	return mWorldPosition;
}

void Transform::translate(const Vector3 &vector)
{
	if (vector.len() > 0.0f)
	{
		setLocalPosition(mLocalPosition.add(vector));
	}
}

void Transform::rotate(const Vector3 &vector)
{
	if (vector.len() > 0.0f)
	{
		setRotation(mRotation.add(vector));
	}
}

void Transform::lookAt(const Vector3 &targetPosition)
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

const Matrix4 &Transform::getTranslationMatrix() const
{
	mTranslationMatrix.translation(getWorldPosition());

	// RenderEngine::getInstance().drawLine(Line(mLocalPosition, Vector3(mLocalPosition.x + 400.0f ,mLocalPosition.y,mLocalPosition.z)));
	// RenderEngine::getInstance().drawLine(Line(mLocalPosition, Vector3(mLocalPosition.x,mLocalPosition.y + 400.0f ,mLocalPosition.z)));
	// RenderEngine::getInstance().drawLine(Line(mLocalPosition, Vector3(mLocalPosition.x,mLocalPosition.y,mLocalPosition.z + 400.0f )));

	return mTranslationMatrix;
}

const Matrix4 &Transform::getRotationMatrix() const
{
	mRotationMatrix.rotation(mRotation);

	return mRotationMatrix;
}

const Matrix4 &Transform::getScaleMatrix() const
{
	mScaleMatrix.scale(mScale);

	return mScaleMatrix;
}

const Matrix4 &Transform::getModelMatrix(bool force /*= false*/)
{
	if (!isStatic() || (isStatic() && !mModelMatrixGenerated) || force)
	{
		mModelMatrix.init(getTranslationMatrix());
		Matrix4 rotationMatrix(getRotationMatrix());
		Matrix4 scaleMatrix(getScaleMatrix());

		scaleMatrix.mul(rotationMatrix);
		mModelMatrix.mul(scaleMatrix);

		mModelMatrixGenerated = true;
	}

	return mModelMatrix;
}

TransformState Transform::getTransformState() const
{
	TransformState transformState(*this);

	return transformState;
}


void Transform::serialize(JSON &json) const
{
	Component::serialize(json);

	DO_SERIALIZE("local_position", mLocalPosition);
	DO_SERIALIZE("scale", mScale);
	DO_SERIALIZE("rotation", mRotation);
}

void Transform::deserialize(const JSON &json)
{
	Component::deserialize(json);


	DO_DESERIALIZE("local_position", mLocalPosition);
	DO_DESERIALIZE("scale", mScale);
	DO_DESERIALIZE("rotation", mRotation);
}