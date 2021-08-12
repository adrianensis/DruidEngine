#pragma once

#include "UI/UIElement.hpp"

CLASS(UIPanel, UIElement)
{
    public:
        virtual void initFromConfig(const UIElementConfig& config) override;
};