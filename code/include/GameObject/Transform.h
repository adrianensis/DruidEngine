#ifndef DE_TRANSFORM_H
#define DE_TRANSFORM_H

#include "Vector3.h"
#include "Component.h"

namespace DE {

class Transform : public Component{

private:

	Vector3 mWorldPosition;
	Vector3 mLocalPosition;

public:

	DE_GENERATE_METADATA(Transform)

	Transform();
	~Transform() override;

	Vector3& getWorldPosition();
	Vector3& getLocalPosition();
	void setWorldPosition(const Vector3& newVector);
	void setLocalPosition(const Vector3& newVector);

};

} /* namespace DE */

#endif /* DE_TRANSFORM_H */
