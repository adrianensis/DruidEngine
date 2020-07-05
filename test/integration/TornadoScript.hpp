#ifndef DE_TORNADOSCRIPT_H
#define DE_TORNADOSCRIPT_H

#include "Script.hpp"
#include "Vector2.hpp"
#include "Element.hpp"

namespace DE {

class Renderer;

class TornadoScript: public Script {

private:
	Renderer* mRenderer;
	Element mElement;

	f32 mDurationTime;
	f32 mDurationTimeCounter;

public:

	DE_CLASS(TornadoScript, Script)
	;

	void init() override;
	void firstStep() override;
	void step() override;
	void terminate() override;

	void onEnterCollision(GameObject *otherGameObject) override;
	void onCollision(GameObject *otherGameObject) override;
	void onExitCollision(GameObject *otherGameObject) override;

};

} /* namespace DE */

#endif /* DE_TORNADOSCRIPT_H */
