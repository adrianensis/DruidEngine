#include "UIElement.h"

#include "Collider.h"
#include "Renderer.h"
#include "List.h"

namespace DE {

// ---------------------------------------------------------------------------

UIElement::UIElement() : GameObject(){
  mCollider = nullptr;
  mRenderer = nullptr;
}

// ---------------------------------------------------------------------------

UIElement::~UIElement() = default;

// ---------------------------------------------------------------------------

void UIElement::onPressed(){
  mOnPressedFunctor.execute();
}

// ---------------------------------------------------------------------------

void UIElement::onReleased(){
  mOnReleasedFunctor.execute();
}

// ---------------------------------------------------------------------------

void UIElement::setOnPressedCallback(std::function<void()> callback){
  mOnPressedFunctor.setCallback(callback);
}

// ---------------------------------------------------------------------------

void UIElement::setOnReleasedCallback(std::function<void()> callback){
  mOnReleasedFunctor.setCallback(callback);
}

// ---------------------------------------------------------------------------

void UIElement::setComponentsCache(){
  mRenderer = getComponents<Renderer>()->get(0);
  mCollider = getComponents<Collider>()->get(0);
}

// ---------------------------------------------------------------------------

} /* namespace DE */
