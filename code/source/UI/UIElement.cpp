#include "UIElement.h"

namespace DE {

// ---------------------------------------------------------------------------

UIElement::UIElement() : GameObject(){

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

} /* namespace DE */
