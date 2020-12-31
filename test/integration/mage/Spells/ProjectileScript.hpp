#ifndef DE_PROJECTILESCRIPT_H
#define DE_PROJECTILESCRIPT_H

#include "Scripting/Script.hpp"
#include "Maths/Vector2.hpp"
#include "Element.hpp"

namespace DE {

class Renderer;

class ProjectileScript: public Script {

private:
	Renderer* mRenderer;
	Element mElement;

	f32 mExplosionTime;
	f32 mExplosionTimeCounter;
	bool mIsExploding;

public:

	DE_CLASS(ProjectileScript, Script)

	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;

	void onEnterCollision(GameObject *otherGameObject) override;
	void onCollision(GameObject *otherGameObject) override;
	void onExitCollision(GameObject *otherGameObject) override;

	void setElement(Element element) {
		mElement = element;
	}
	Element getElement() const {
		return mElement;
	}

	void explode();
	bool isExploded() const {
		return mIsExploding;
	}

};

} /* namespace DE */

#endif /* DE_PROJECTILESCRIPT_H */
