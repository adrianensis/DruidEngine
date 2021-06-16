#pragma once

#include "UI/UIElement.hpp"

CLASS(UIPanel, UIElement) {

public:
	
	virtual void setText(const std::string& text) override { /* panels have no text, they're just background panels */ };
};