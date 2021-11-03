#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Matrix4.hpp"

class Transform: public Component
{
    GENERATE_METADATA(Transform)
	PRI Vector3 mWorldPosition = {};

	PRI mutable Matrix4 mModelMatrix = {};
	PRI mutable Matrix4 mTranslationMatrix = {};
	PRI mutable Matrix4 mRotationMatrix = {};
	PRI mutable Matrix4 mScaleMatrix = {};

	PRI bool mModelMatrixGenerated = {};

	PRI mutable bool mIsDirtyTranslation = {};
	PRI mutable bool mIsDirtyRotation = {};
	PRI mutable bool mIsDirtyScale = {};

	PRI bool mForceModelMatrixCalculation = {};

	//Transform* mParent;
	PRI Transform * mParent = {}; GET_SET(Parent);

	PRI Vector3 mLocalPosition = {}; GET(LocalPosition)
	PRI Vector3 mRotation = {}; GET(Rotation)
	PRI Vector3 mScale = {}; GET(Scale)
	PRI bool mAffectedByProjection = {}; GET_SET(AffectedByProjection)

PRI

	Vector3 calculateWorldPosition() const;

PUB
	static const Vector3 smRight;
	static const Vector3 smUp;
	static const Vector3 smForward;

	Transform();
	~Transform() override;

	void init() override;

	/*!
	\brief Translate the object by an increment vector.
	\param Vector3 Delta position.
	*/
	void translate(const Vector3 &vector);

	/*!
	\brief Rotate the object by an increment vector.
	\param Vector3 Delta rotation.
	*/
	void rotate(const Vector3 &vector);

	/*!
	\brief Orient the object to a 3D point.
	\param Vector3 Target position.
	*/
	void lookAt(const Vector3 &targetPosition);

	// void setWorldPosition(const Vector3& vector);
	void setLocalPosition(const Vector3 &vector);
	void setRotation(const Vector3 &vector);
	void setScale(const Vector3 &vector);

	const Vector3 &getWorldPosition();

	const Matrix4 &getTranslationMatrix();
	const Matrix4 &getRotationMatrix();
	const Matrix4 &getScaleMatrix();

	const Matrix4 &getModelMatrix(bool force = false);

	bool isDirtyTranslation() const;
	void setDirtyTranslation(bool dirty);

	void forceModelMatrixCalculation()
	{
		mForceModelMatrixCalculation = true;
	}

	SERIALIZE();
	DESERIALIZE();
};