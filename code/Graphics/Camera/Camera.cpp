#include "Graphics/Camera/Camera.hpp"

#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Camera/Frustum.hpp"
#include "Graphics/RenderContext.hpp"

Camera::Camera()
{
	mFrustum = nullptr;

	mIsOrtho = true;
	mZoom = 1;
}

Camera::~Camera()
{
	DELETE(mFrustum);
}

void Camera::init()
{
	TRACE()

	mViewTranslationMatrix.identity();

	mInversePVMatrix.identity();

	mFrustum = NEW(Frustum);
	mFrustum->init(this);
};

/**
 * Return the frustum.
 * @returns {Frustum} The frustum.
 */
// const Frustum* Camera::getFrustum(){
// 	return mFrustum;
// };
void Camera::setOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
{
	mIsOrtho = true;
	
	mLeft = left;
	mRight = right;
	mBottom = bottom;
	mTop = top;
	mNear = near;
	mFar = far;

	mProjectionMatrix.ortho(mLeft * RenderContext::getAspectRatio(), mRight * RenderContext::getAspectRatio(), mBottom,
							mTop, mNear, mFar);

	VAR(f32, mLeft * RenderContext::getAspectRatio())
	VAR(f32, mRight * RenderContext::getAspectRatio())
	VAR(f32, mBottom)
	VAR(f32, mTop)
	VAR(f32, mNear)
	VAR(f32, mFar)

	calculateInverseMatrix(true);
	mFrustum->build(true);
};

void Camera::setPerspective(f32 near, f32 far, f32 aspect, f32 fov)
{
	mIsOrtho = false;
	mProjectionMatrix.perspective(near, far, aspect, fov);

	calculateInverseMatrix(true);
	mFrustum->build(true);
};

const Matrix4 &Camera::getProjectionMatrix() const
{
	return mProjectionMatrix;
};

const Matrix4 &Camera::getViewTranslationMatrix()
{
	Vector3 position = getGameObject()->getTransform()->getWorldPosition();
	mViewTranslationMatrix.translation(position * -1);

	return mViewTranslationMatrix;
};

const Matrix4 &Camera::getViewRotationMatrix()
{
	return getGameObject()->getTransform()->getRotationMatrix();
};

Vector3 Camera::screenToWorld(Vector2 screenPosition)
{
	Vector4 v = mInversePVMatrix.mulVector(Vector4(screenPosition.x, screenPosition.y, 0, 1.0));

	v.x = v.x / v.w;
	v.y = v.y / v.w;
	v.z = v.z / v.w;

	return Vector3(v.x, v.y, v.z);
}

void Camera::calculateInverseMatrix(bool forceCalculate /*= false*/)
{
	Transform *transform = getGameObject()->getTransform();

	if (forceCalculate || transform->isDirtyTranslation())
	{
		Matrix4 inverseProjectionMatrix;
		Matrix4 viewTranslationMatrix;
		Matrix4 viewRotationMatrix;

		inverseProjectionMatrix.init(getProjectionMatrix());
		viewTranslationMatrix.init(getViewTranslationMatrix());
		viewRotationMatrix.init(getViewRotationMatrix());

		viewTranslationMatrix.mul(viewRotationMatrix);
		inverseProjectionMatrix.mul(viewTranslationMatrix);

		inverseProjectionMatrix.invert();

		mInversePVMatrix.init(inverseProjectionMatrix);

		// HACK: set the dirty value again
		transform->setDirtyTranslation(true);
	}
}

void Camera::setZoom(f32 zoom)
{
	mZoom = zoom;

	if (mIsOrtho)
	{
		setOrtho(mLeft * mZoom, mRight * mZoom, mBottom * mZoom, mTop * mZoom, mNear, mFar);
	}
	else
	{
		// setPerspective
	}
}