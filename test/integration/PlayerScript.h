#ifndef DE_PLAYERSCRIPT_H
#define DE_PLAYERSCRIPT_H

#include "Script.h"
#include "Vector2.h"

namespace DE {

  class UIText;

class PlayerScript : public Script{
public:

  UIText* textCollision;
  UIText* textExit;
  UIText* textEnter;

DE_CLASS(PlayerScript, Script);

void init() override;
void firstStep() override;
void step() override;
void terminate() override;

void onEnterCollision(GameObject* otherGameObject) override;
void onCollision(GameObject* otherGameObject) override;
void onExitCollision(GameObject* otherGameObject) override;

};

} /* namespace DE */

#endif /* DE_PLAYERSCRIPT_H */
