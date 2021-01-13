#ifndef DE_EDITORBUILDER_H
#define DE_EDITORBUILDER_H

#include "Maths/Vector2.hpp"
#include <string>
#include "UI/UIElement.hpp"
#include "Core/Singleton.hpp"

namespace DE {

class UIButton;
class UIDropdown;
class UIText;
class UITextEditable;

// Vector2 Text Editable
class TextEditableVector2 {
public:
	UITextEditable* TextEditableX = nullptr;
	UITextEditable* TextEditableY = nullptr;
};

class EditorBuilder : public DE_Class, public Singleton<EditorBuilder>{

public:

	DE_CLASS(EditorBuilder)

	u32 mUILayer = 3;

	UIElement* createPanel(const Vector2& position, const Vector2& size);
	UIText* createLabel(const std::string& text);
	UITextEditable* createTextBoxSimple(const std::string& text, UIElementCallback onTextChangedCallback);
	UITextEditable* createTextBoxLabeled(const std::string& textLabel, const std::string& text, UIElementCallback onTextChangedCallback);
	UIButton* createButton(const std::string& textLabel, UIElementCallback onPressedCallback);
	UIButton* createRadialButton(const std::string& textLabel, UIElementCallback onPressedCallback);
	TextEditableVector2 createTextBoxVector2(const std::string& textLabel, UIElementCallback onTextChangedCallbackX, UIElementCallback onTextChangedCallbackY);
	UIDropdown* createDropdown(const std::string& textLabel);
};

} /* namespace DE */

#endif /* DE_EDITORBUILDER_H */
