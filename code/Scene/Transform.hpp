#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Matrix4.hpp"

CLASS(Transform, Component)
{
	PRI_M(Vector3, WorldPosition, NONE)

	PRI_M(mutable Matrix4, ModelMatrix, NONE)
	PRI_M(mutable Matrix4, TranslationMatrix, NONE)
	PRI_M(mutable Matrix4, RotationMatrix, NONE)
	PRI_M(mutable Matrix4, ScaleMatrix, NONE)

	PRI_M(bool, ModelMatrixGenerated, NONE)

	PRI_M(mutable bool, IsDirtyTranslation, NONE)
	PRI_M(mutable bool, IsDirtyRotation, NONE)
	PRI_M(mutable bool, IsDirtyScale, NONE)

	PRI_M(bool, ForceModelMatrixCalculation, NONE)

	//Transform* mParent;
	PRI_M(Transform *, Parent, GET_SET);

	PRI_M(Vector3, LocalPosition, GET)
	PRI_M(Vector3, Rotation, GET)
	PRI_M(Vector3, Scale, GET)
	PRI_M(bool, AffectedByProjection, GET_SET)

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