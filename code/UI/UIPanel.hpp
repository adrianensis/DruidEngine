#pragma once

#include "UI/UIElement.hpp"

CLASS(UIPanel, UIElement)
{
    public:
        VIR void initFromConfig(CNS UIElementConfig& config) OVR;
};