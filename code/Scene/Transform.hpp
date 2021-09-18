#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Matrix4.hpp"

CLASS(Transform, Component)
{
	PRI(WorldPosition, NONE, Vector3)

	PRI(ModelMatrix, NONE, mutable Matrix4)
	PRI(TranslationMatrix, NONE, mutable Matrix4)
	PRI(RotationMatrix, NONE, mutable Matrix4)
	PRI(ScaleMatrix, NONE, mutable Matrix4)

	PRI(ModelMatrixGenerated, NONE, bool)

	PRI(IsDirtyTranslation, NONE, mutable bool)
	PRI(IsDirtyRotation, NONE, mutable bool)
	PRI(IsDirtyScale, NONE, mutable bool)

	PRI(ForceModelMatrixCalculation, NONE, bool)

	//Transform* mParent;
	PRI(Parent, GET_SET, Transform *);

	PRI(LocalPosition, GET, Vector3)
	PRI(Rotation, GET, Vector3)
	PRI(Scale, GET, Vector3)
	PRI(AffectedByProjection, GET_SET, bool)

private:

	Vector3 calculateWorldPosition() CNS;

public:
	STC CNS Vector3 smRight;
	STC CNS Vector3 smUp;
	STC CNS Vector3 smForward;

	Transform();
	~Transform();

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