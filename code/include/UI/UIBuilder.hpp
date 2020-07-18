#ifndef DE_UIBUILDER_H
#define DE_UIBUILDER_H

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

enum class UIElementType {
	BUTTON,
	TEXT,
	TEXTBOX
};

class UIElementData: public DE_Class {
public:

	DE_CLASS(UIElementData, DE_Class);

	Vector2 mPosition;
	Vector2 mSize;
	std::string mText;
	u32 mLayer;

	void init(const Vector2 &position, const Vector2 &size, const std::string& text, u32 layer);
};

class UIBuilder: public DE_Class, public Singleton<UIBuilder> {
private:

	Material* mButtonMaterial;

	UIButton* createButton(Scene *scene, const UIElementData& data);
	UIText* createText(Scene *scene, const UIElementData& data);
	UIText* createTextBox(Scene *scene, const UIElementData& data);
	//UIList* createList(Scene *scene, const Vector2 &position, const Vector2 &size, u32 layer);

public:

	DE_CLASS(UIBuilder, DE_Class);

	UIElement* const create(Scene *scene, UIElementData data, UIElementType type);
};

} /* namespace DE */

#endif /* DE_UIBUILDER_H */
