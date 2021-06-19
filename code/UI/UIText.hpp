#pragma once

#include "UI/UIElement.hpp"
#include "Maths/Vector2.hpp"
#include <string>

CLASS(UIText, UIElement) {

	PRI(Layer, NONE, u32)
	PRI(Size, NONE, Vector2)
	PRI(String, NONE, std::string)

	PRI(FontRenderers, NONE, std::vector<Renderer*>)

public:
	
	virtual void init() override;
	virtual void onDestroy() override;

	virtual void setText(const std::string& text) override;

	void setLayer(u32 layer) {
		mLayer = layer;
	};

	void setSize(const Vector2 &size) {
		mSize = size;
	};

	const std::string& getText() const {
		return mString;
	};
};