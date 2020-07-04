#ifndef DE_SCRIPT_H
#define DE_SCRIPT_H

#include "Component.hpp"

namespace DE {

class GameObject;

class Script: public Component {
private:
	bool mFirstStepDone;

public:

	DE_CLASS(Script, Component)
	;

	virtual void init();
	virtual void firstStep();
	virtual void step();
	virtual void terminate();

	virtual void onEnterCollision(GameObject *otherGameObject);
	virtual void onCollision(GameObject *otherGameObject);
	virtual void onExitCollision(GameObject *otherGameObject);

	bool isFirstStepDone() const {
		return mFirstStepDone;
	}
	void firstStepDone() {
		mFirstStepDone = true;
	}
};

} /* namespace DE */

#endif /* DE_SCRIPT_H */
