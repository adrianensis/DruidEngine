#ifndef DE_ENEMYSCRIPT_H
#define DE_ENEMYSCRIPT_H

#include "Script.hpp"
#include "Vector2.hpp"
#include "Element.hpp"

namespace DE {

class UIText;
class Renderer;
class RigidBody;

class EnemyScript: public Script {

private:
	Renderer* mRenderer;
	RigidBody* mRigidBody;
	Element mElement;
	bool mFalling;
	bool mSlowDown;
	f32 mSpeed;

public:

	DE_CLASS(EnemyScript, Script)
	;

	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;

	void onEnterCollision(GameObject *otherGameObject) override;
	void onCollision(GameObject *otherGameObject) override;
	void onExitCollision(GameObject *otherGameObject) override;

	void createProjectile(f32 x, f32 y, f32 clickX, f32 clickY);

	void setElement(Element element) {
		mElement = element;
	}

};

} /* namespace DE */

#endif /* DE_ENEMYSCRIPT_H */
