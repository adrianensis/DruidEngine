#ifndef DE_ENEMYSCRIPT_H
#define DE_ENEMYSCRIPT_H

#include "Script.h"
#include "Vector2.h"

namespace DE {

  class UIText;
  class Renderer;
  class RigidBody;

class EnemyScript : public Script{

private:
  Renderer* mRenderer;
  RigidBody* mRigidBody;

public:

DE_CLASS(EnemyScript, Script);

void init() override;
void firstStep() override;
void step() override;
void terminate() override;

void onEnterCollision(GameObject* otherGameObject) override;
void onCollision(GameObject* otherGameObject) override;
void onExitCollision(GameObject* otherGameObject) override;

void createProjectile(f32 x, f32 y, f32 clickX, f32 clickY);

};

} /* namespace DE */

#endif /* DE_ENEMYSCRIPT_H */
