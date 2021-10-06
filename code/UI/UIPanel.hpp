#pragma once

#include "UI/UIElement.hpp"

CLASS(UIPanel, UIElement)
{
    public:
        VIR void initFromConfig(const UIElementConfig& config) OVR;
};