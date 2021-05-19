#pragma once

#include "Scene/Component.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Matrix4.hpp"

CLASS(Transform, Component) {

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
	PRI(Parent, GET_SET, Transform*);

	PRI(LocalPosition, GET, Vector3)
	PRI(Rotation, GET, Vector3)
	PRI(Scale, GET, Vector3)
	PRI(AffectedByProjection, GET_SET, bool)

public:

	static const Vector3 smRight;
	static const Vector3 smUp;
	static const Vector3 smForward;

	Transform();

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

	const Vector3& getWorldPosition();

	const Matrix4& getTranslationMatrix();
	const Matrix4& getRotationMatrix();
	const Matrix4& getScaleMatrix();

	const Matrix4& getModelMatrix(bool force = false);

	bool isDirtyTranslation() const;
	void setDirtyTranslation(bool dirty);

	void forceModelMatrixCalculation() {
		mForceModelMatrixCalculation = true;
	}

	
};