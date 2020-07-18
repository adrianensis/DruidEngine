#ifndef DE_UI_H
#define DE_UI_H

#include "DE_Class.hpp"
#include "Singleton.hpp"
#include "Vector2.hpp"
#include "UIElement.hpp"
#include "UIButton.hpp"
#include "UIText.hpp"
#include "UIList.hpp"

#include <string>

namespace DE {

class Texture;
class Material;
class Scene;

template<class T> class List;
template<class K, class V> class HashMap;

class UI: public DE_Class, public Singleton<UI> {
private:

	Texture* mButtonTexture;
	Material* mButtonMaterial;

	Texture* mFontTexture;
	Material* mFontMaterial;

	List<UIElement*>* mUIElements;
	HashMap<c8, Vector2>* mCharMap;

	Vector2 mFontTilesCount;
	Vector2 mFontTileTextureSize;

public:

	class UIElementData: public DE_Class {
	public:

		DE_CLASS(UIElementData, DE_Class);

		Vector2 mPosition;
		Vector2 mSize;
		std::string mText;
		u32 mLayer;
	};

	DE_CLASS(UI, DE_Class);

	UIButton* createButton(Scene *scene, const Vector2 &position, const Vector2 &size, u32 layer);
	UIText* createText(Scene *scene, const Vector2 &position, const Vector2 &size, const std::string &text, u32 layer);
	UIText* createTextBox(Scene *scene, const Vector2 &position, const Vector2 &size, const std::string &text, u32 layer);
	UIList* createList(Scene *scene, const Vector2 &position, const Vector2 &size, u32 layer);

	Vector2 getCharTextureCoordinates(c8 character);
	Vector2 getFontTileTextureSize() const {
		return mFontTileTextureSize;
	};

	Material* getFontMaterial() const {
		return mFontMaterial;
	};


	void init();
	void step();
	void terminate();

};

} /* namespace DE */

#endif /* DE_UI_H */
