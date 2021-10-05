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

	Vector3 calculateWorldPosition() CNS;

public:
	STC CNS Vector3 smRight;
	STC CNS Vector3 smUp;
	STC CNS Vector3 smForward;

	Transform();
	~Transform() OVR;

	void init() OVR;

	/*!
	\brief Translate the object by an increment vector.
	\param Vector3 Delta position.
	*/
	void translate(CNS Vector3 &vector);

	/*!
	\brief Rotate the object by an increment vector.
	\param Vector3 Delta rotation.
	*/
	void rotate(CNS Vector3 &vector);

	/*!
	\brief Orient the object to a 3D point.
	\param Vector3 Target position.
	*/
	void lookAt(CNS Vector3 &targetPosition);

	// void setWorldPosition(CNS Vector3& vector);
	void setLocalPosition(CNS Vector3 &vector);
	void setRotation(CNS Vector3 &vector);
	void setScale(CNS Vector3 &vector);

	CNS Vector3 &getWorldPosition();

	CNS Matrix4 &getTranslationMatrix();
	CNS Matrix4 &getRotationMatrix();
	CNS Matrix4 &getScaleMatrix();

	CNS Matrix4 &getModelMatrix(bool force = false);

	bool isDirtyTranslation() CNS;
	void setDirtyTranslation(bool dirty);

	void forceModelMatrixCalculation()
	{
		mForceModelMatrixCalculation = true;
	}

	SERIALIZE();
	DESERIALIZE();
};