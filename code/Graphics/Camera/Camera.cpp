#include "Graphics/Camera/Camera.hpp"

#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Camera/Frustum.hpp"
#include "Graphics/RenderContext.hpp"

#include "Profiler/Profiler.hpp"

Camera::Camera()
{
	mIsOrtho = true;
	mZoom = 1;
}

Camera::~Camera()
{
}

void Camera::init()
{
	TRACE()

	mViewMatrix.identity();

	mInversePVMatrix.identity();

	mFrustum.init(this);
}

void Camera::onComponentAdded()
{
	recalculatePerspectiveMatrix();

	mTransformState = getGameObject()->getTransform()->getTransformState();
}

void Camera::update()
{
	PROFILER_TIMEMARK_START()

	TransformState currentTransformState = getGameObject()->getTransform()->getTransformState();
	if(!currentTransformState.eq(mTransformState))
	{
		mFrustum.build();

		mInversePVMatrixNeedsUpdate = true;
		mViewMatrixNeedsUpdate = true;
		mProjectionViewMatrixNeedsUpdate = true;

		mTransformState = currentTransformState;
	}

	PROFILER_TIMEMARK_END()
}

void Camera::recalculatePerspectiveMatrix()
{
	if (mIsOrtho)
	{
		setOrtho(mLeft, mRight, mBottom, mTop, mNear, mFar);
	}
	else
	{
		setPerspective(mNear, mFar, mAspect, mFov);
	}

	mProjectionViewMatrixNeedsUpdate = true;
	calculateInverseMatrix(true);
	mFrustum.build();
}

void Camera::setOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
{
	mIsOrtho = true;
	
	mLeft = left;
	mRight = right;
	mBottom = bottom;
	mTop = top;
	mNear = near;
	mFar = far;

	mProjectionMatrix.ortho(mLeft * RenderContext::getAspectRatio() * mZoom, mRight * RenderContext::getAspectRatio() * mZoom, mBottom* mZoom,
							mTop* mZoom, mNear, mFar);
};

void Camera::setPerspective(f32 near, f32 far, f32 aspect, f32 fov)
{
	mIsOrtho = false;

	mNear = near;
	mFar = far;
	mAspect = aspect;
	mFov = fov;
	
	mProjectionMatrix.perspective(mNear, mFar, mAspect, mFov);
};

void Camera::onResize()
{
	recalculatePerspectiveMatrix();
}

const Matrix4 &Camera::getProjectionMatrix() const
{
	return mProjectionMatrix;
};

const Matrix4 &Camera::getViewMatrix() const
{
	if(mViewMatrixNeedsUpdate)
	{
		const Vector3& position = getGameObject()->getTransform()->getWorldPosition();
		mViewMatrix.translation(position * -1);
		mViewMatrix.mul(getGameObject()->getTransform()->getRotationMatrix());

		mViewMatrixNeedsUpdate = false;
	}

	return mViewMatrix;
};

const Matrix4 &Camera::getProjectionViewMatrix() const
{
	if(mProjectionViewMatrixNeedsUpdate)
	{
		mProjectionViewMatrix.init(getProjectionMatrix());
		mProjectionViewMatrix.mul(getViewMatrix());
		mProjectionViewMatrixNeedsUpdate = false;
	}

	return mProjectionViewMatrix;
}

Vector3 Camera::screenToWorld(const Vector2& screenPosition)
{	
	calculateInverseMatrix();
	Vector4 v = mInversePVMatrix.mulVector(Vector4(screenPosition.x, screenPosition.y, 0, 1.0));

	v.x = v.x / v.w;
	v.y = v.y / v.w;
	v.z = v.z / v.w;

	return v;
}

void Camera::calculateInverseMatrix(bool force /*= false*/)
{
	if(mInversePVMatrixNeedsUpdate || force)
	{
		Matrix4 inverseProjectionMatrix;
		mInversePVMatrix.init(getProjectionViewMatrix());		
		mInversePVMatrix.invert();

		mInversePVMatrixNeedsUpdate = false;
	}
}

void Camera::setZoom(f32 zoom)
{
	mZoom = zoom;
	recalculatePerspectiveMatrix();
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