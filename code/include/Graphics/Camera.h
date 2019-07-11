#ifndef DE_CAMERA_H
#define DE_CAMERA_H

#include "Component.h"
#include "Memory.h"

namespace DE {

class Matrix4;

class Camera : public Component{

private:

	Matrix4* mProjectionMatrix;
	Matrix4* mViewMatrix;
	//Frustum* mFrustum;

public:

	Camera();
	~Camera() override;

	DE_GENERATE_METADATA(Camera);

	virtual void init() override;

	const Matrix4* getProjectionMatrix() const;
	const Matrix4* getViewMatrix();
	void setProjection(Matrix4* projectionMatrix);

	void setOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
	void setPerspective(f32 near, f32 far, f32 aspect, f32 fov);
};

} /* namespace DE */

#endif /* DE_CAMERA_H */
