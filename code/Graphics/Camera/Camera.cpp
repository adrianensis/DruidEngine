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

void Camera::recalculate()
{
	if (mIsOrtho)
	{
		setOrtho(mLeft, mRight, mBottom, mTop, mNear, mFar);
	}
	else
	{
		setPerspective(mNear, mFar, mAspect, mFov);
	}
}

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

	mProjectionMatrix.ortho(mLeft * RenderContext::getAspectRatio() * mZoom, mRight * RenderContext::getAspectRatio() * mZoom, mBottom * mZoom,
							mTop * mZoom, mNear, mFar);

	calculateInverseMatrix(true);
	mFrustum->build(true);
};

void Camera::setPerspective(f32 near, f32 far, f32 aspect, f32 fov)
{
	mIsOrtho = false;

	mNear = near;
	mFar = far;
	mAspect = aspect;
	mFov = fov;
	
	mProjectionMatrix.perspective(mNear, mFar, mAspect, mFov);

	calculateInverseMatrix(true);
	mFrustum->build(true);
};

void Camera::onResize()
{
	recalculate();
}

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
		inverseProjectionMatrix.invert();
		viewTranslationMatrix.init(getViewTranslationMatrix());
		viewRotationMatrix.init(getViewRotationMatrix());

		viewTranslationMatrix.mul(viewRotationMatrix);

		inverseProjectionMatrix.mul(viewTranslationMatrix);
		
		mInversePVMatrix.init(inverseProjectionMatrix);

		// HACK: set the dirty value again
		transform->setDirtyTranslation(true);
	}
}

void Camera::setZoom(f32 zoom)
{
	mZoom = zoom;
	recalculate();
}

void Camera::zoomIn(f32 zoomDelta)
{
	f32 newZoom = mZoom - zoomDelta;

	if(newZoom < 0)
	{
		newZoom = 0;
	}

	setZoom(newZoom);
}

void Camera::zoomOut(f32 zoomDelta)
{
	f32 newZoom = mZoom + zoomDelta;
	setZoom(newZoom);
}