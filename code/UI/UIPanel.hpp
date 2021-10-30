#pragma once

#include "UI/UIElement.hpp"

CLASS(UIPanel, UIElement)
{
    PUB
        virtual void initFromConfig(const UIElementConfig& config) override;
};