#ifndef DE_UI_H
#define DE_UI_H

#include "DE_Class.h"
#include "Singleton.h"
#include "UIButton.h"
#include "Vector2.h"

namespace DE {

class UIElement;
class Texture;
class Material;

template <class T> class List;

class UI : public DE_Class, public Singleton<UI> {
private:

Texture* mTexture;
Material* mMaterial;

List<UIElement*>* mUIElements;

public:

DE_CLASS(UI, DE_Class);

UIButton* createButton(const Vector2& position, const Vector2& size);

void init();
void step();
void terminate();

};

} /* namespace DE */

#endif /* DE_UI_H */
