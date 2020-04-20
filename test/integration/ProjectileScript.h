#ifndef DE_PROJECTILESCRIPT_H
#define DE_PROJECTILESCRIPT_H

#include "Script.h"
#include "Vector2.h"

namespace DE {

  class Renderer;

class ProjectileScript : public Script{

private:
  Renderer* mRenderer;

public:

DE_CLASS(ProjectileScript, Script);

void init() override;
void firstStep() override;
void step() override;
void terminate() override;

void onEnterCollision(GameObject* otherGameObject) override;
void onCollision(GameObject* otherGameObject) override;
void onExitCollision(GameObject* otherGameObject) override;


};

} /* namespace DE */

#endif /* DE_PROJECTILESCRIPT_H */
