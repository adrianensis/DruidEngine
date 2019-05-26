#ifndef DE_TRANSFORM_H
#define DE_TRANSFORM_H

#include "Component.h"
#include "Vector3.h"

namespace DE {

class Matrix4;

class Transform : public Component{

private:

	Vector3 mWorldPosition;
	Vector3 mLocalPosition;

	Vector3 mRotation;
	Vector3 mScale;

	Matrix4* mMatrix;
	Matrix4* mTranslationMatrix;
	Matrix4* mRotationMatrix;
	Matrix4* mScaleMatrix;

	bool mIsirty;

public:

	static const Vector3 smRight;
	static const Vector3 smUp;
	static const Vector3 smForward;

	DE_GENERATE_METADATA(Transform)

	Transform();
	~Transform() override;

	const Vector3& getWorldPosition();
	const Vector3& getLocalPosition();
	const Vector3& getRotation();
	const Vector3& getScale();

	void setWorldPosition(const Vector3& vector);
	void setLocalPosition(const Vector3& vector);
	void setRotation(const Vector3& vector);
	void setScale(const Vector3& vector);

	void translate(const Vector3& vector);
	void rotate(const Vector3& vector);
	void lookAt(const Vector3& vector);
};

} /* namespace DE */

#endif /* DE_TRANSFORM_H */
