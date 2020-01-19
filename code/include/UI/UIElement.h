#ifndef DE_UIELEMENT_H
#define DE_UIELEMENT_H

#include "GameObject.h"
#include "Functor.h"

namespace DE {

  class Collider;
  class Renderer;

class UIElement : public GameObject {

private:

  Functor mOnPressedFunctor;
  Functor mOnReleasedFunctor;

  Collider* mCollider;
  Renderer* mRenderer;

  bool mPressed;

public:

DE_CLASS(UIElement, GameObject);

void onPressed();
void onReleased();

void setOnPressedCallback(std::function<void()> callback);
void setOnReleasedCallback(std::function<void()> callback);

DE_GET(Collider*, Collider);
DE_GET(Renderer*, Renderer);

DE_GET(bool, Pressed);

void setComponentsCache();

};

} /* namespace DE */

#endif /* DE_UIELEMENT_H */
