#ifndef DE_PLAYERSCRIPT_H
#define DE_PLAYERSCRIPT_H

#include "Script.h"
#include "Vector2.h"
#include "Vector4.h"

namespace DE {

  class UIText;
  class Renderer;
  class RigidBody;

class PlayerScript : public Script{

private:
  Renderer* mRenderer;
  RigidBody* mRigidBody;
  f32 mAttackAnimTime;
  f32 mAttackAnimTimeCount;
  bool mIsAttackPlaying;

  GameObject* mBook;
  Vector4 mBookColor;

public:

DE_CLASS(PlayerScript, Script);

void init() override;
void firstStep() override;
void step() override;
void terminate() override;

void onEnterCollision(GameObject* otherGameObject) override;
void onCollision(GameObject* otherGameObject) override;
void onExitCollision(GameObject* otherGameObject) override;

void createProjectile(f32 x, f32 y, f32 clickX, f32 clickY);
void createBook(f32 x, f32 y, const Vector4& color);

void setBookColor(const Vector4& color) { mBookColor=color; }

};

} /* namespace DE */

#endif /* DE_PLAYERSCRIPT_H */
