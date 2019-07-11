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
	mutable Matrix4* mTranslationMatrix;
	mutable Matrix4* mRotationMatrix;
	mutable Matrix4* mScaleMatrix;

	mutable bool mIsDirtyTranslation;
	mutable bool mIsDirtyRotation;
	mutable bool mIsDirtyScale;

public:

	static const Vector3 smRight;
	static const Vector3 smUp;
	static const Vector3 smForward;

	DE_GENERATE_METADATA(Transform);

	Transform();
	~Transform() override;

	virtual void init() override;

	void translate(const Vector3& vector);
	void rotate(const Vector3& vector);
	void lookAt(const Vector3& vector);

	void setWorldPosition(const Vector3& vector);
	void setLocalPosition(const Vector3& vector);
	void setRotation(const Vector3& vector);
	void setScale(const Vector3& vector);

	const Vector3& getWorldPosition() const;
	const Vector3& getLocalPosition() const;
	const Vector3& getRotation() const;
	const Vector3& getScale() const;

	const Matrix4& getTranslationMatrix() const;
	const Matrix4& getRotationMatrix() const;
	const Matrix4& getScaleMatrix() const;

};

} /* namespace DE */

#endif /* DE_TRANSFORM_H */
