#ifndef DE_UIELEMENT_H
#define DE_UIELEMENT_H

#include "GameObject.h"
#include "Functor.h"

namespace DE {

class UIElement : public GameObject {

private:

  Functor mOnPressedFunctor;
  Functor mOnReleasedFunctor;

public:

DE_CLASS(UIElement, GameObject);

void onPressed();
void onReleased();

void setOnPressedCallback(std::function<void()> callback);
void setOnReleasedCallback(std::function<void()> callback);

};

} /* namespace DE */

#endif /* DE_UIELEMENT_H */
