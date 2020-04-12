#ifndef DE_UITEXT_H
#define DE_UITEXT_H

#include "UIElement.h"
#include "Vector2.h"
#include <string>

namespace DE {

class UIText : public UIElement {

  u32 mLayer;
  Vector2 mSize;
  std::string mString;

public:

DE_CLASS(UIText, UIElement);

void setText(const std::string& text);

void setLayer(u32 layer) { mLayer = layer; };
void setSize(const Vector2& size) { mSize = size; };

};

} /* namespace DE */

#endif /* DE_UITEXT_H */
