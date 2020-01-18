#ifndef DE_UI_H
#define DE_UI_H

#include "DE_Class.h"
#include "Singleton.h"
#include "Vector2.h"
#include <string>

namespace DE {

class UIElement;
class UIButton;
class UIText;
class Texture;
class Material;

template <class T> class List;
template <class K, class V> class HashMap;

class UI : public DE_Class, public Singleton<UI> {
private:

Texture* mButtonTexture;
Material* mButtonMaterial;

Texture* mFontTexture;
Material* mFontMaterial;

List<UIElement*>* mUIElements;
HashMap<c8, Vector2>* mCharMap;

f32 mFontTilesCount;
f32 mFontTileTextureSize;

public:

DE_CLASS(UI, DE_Class);

UIButton* createButton(const Vector2& position, const Vector2& size);
UIText* createText(const Vector2& position, const Vector2& size, const std::string& text);

void init();
void step();
void terminate();

};

} /* namespace DE */

#endif /* DE_UI_H */
