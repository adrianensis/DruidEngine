#include "UIElement.h"

#include "Collider.h"
#include "Renderer.h"
#include "List.h"

namespace DE {

// ---------------------------------------------------------------------------

UIElement::UIElement() : GameObject(){
  mCollider = nullptr;
  mRenderer = nullptr;
  mPressed = false;
}

// ---------------------------------------------------------------------------

UIElement::~UIElement() = default;

// ---------------------------------------------------------------------------

void UIElement::onPressed(){
  mPressed = true;
  //mRenderer->setColor(Vector4(1.0f,1.0f,0.0f,1.0f));
  mOnPressedFunctor.execute();
}

// ---------------------------------------------------------------------------

void UIElement::onReleased(){
  mPressed = false;
  //mRenderer->setColor(Vector4(0.0f,0.0f,0.0f,1.0f));
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
  mRenderer = getComponents<Renderer>() ? getComponents<Renderer>()->get(0) : nullptr;
  mCollider = getComponents<Collider>() ? getComponents<Collider>()->get(0) : nullptr;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
