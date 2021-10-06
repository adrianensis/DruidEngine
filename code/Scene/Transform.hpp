#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Matrix4.hpp"

CLASS(Transform, Component)
{
	PRI(Vector3, WorldPosition, NONE)

	PRI(mutable Matrix4, ModelMatrix, NONE)
	PRI(mutable Matrix4, TranslationMatrix, NONE)
	PRI(mutable Matrix4, RotationMatrix, NONE)
	PRI(mutable Matrix4, ScaleMatrix, NONE)

	PRI(bool, ModelMatrixGenerated, NONE)

	PRI(mutable bool, IsDirtyTranslation, NONE)
	PRI(mutable bool, IsDirtyRotation, NONE)
	PRI(mutable bool, IsDirtyScale, NONE)

	PRI(bool, ForceModelMatrixCalculation, NONE)

	//Transform* mParent;
	PRI(Transform *, Parent, GET_SET);

	PRI(Vector3, LocalPosition, GET)
	PRI(Vector3, Rotation, GET)
	PRI(Vector3, Scale, GET)
	PRI(bool, AffectedByProjection, GET_SET)

private:

	Vector3 calculateWorldPosition() const;

public:
	static const Vector3 smRight;
	static const Vector3 smUp;
	static const Vector3 smForward;

	Transform();
	~Transform() OVR;

	void init() OVR;

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