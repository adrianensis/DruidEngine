#ifndef DE_UI_H
#define DE_UI_H

#include "DE_Class.hpp"
#include "Singleton.hpp"
#include "Vector2.hpp"
#include "UIElement.hpp"
#include "UIButton.hpp"
#include "UIText.hpp"
#include "UIList.hpp"
#include "UIBuilder.hpp"

#include <string>

namespace DE {

class Texture;
class Material;
class Scene;

template<class T> class List;
template<class K, class V> class HashMap;

class UI: public DE_Class, public Singleton<UI> {
private:

	List<UIElement*>* mUIElements;
	HashMap<c8, Vector2>* mCharMap;

	Vector2 mFontTilesCount;
	Vector2 mFontTileTextureSize;

	Material* mFontMaterial;

public:

	DE_CLASS(UI, DE_Class);

	Vector2 getCharTextureCoordinates(c8 character);
	Vector2 getFontTileTextureSize() const {
		return mFontTileTextureSize;
	};

	UIBuilder* const getBuilder() const;
	Material* getFontMaterial();

	void init();
	void addUIElement(UIElement* uiElement);
	void step();
	void terminate();

};

} /* namespace DE */

#endif /* DE_UI_H */
