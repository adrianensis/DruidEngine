#include "UI/UIEditableText.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Material/Material.hpp"
#include "Maths/Vector3.hpp"
#include "UI/UI.hpp"
#include "Scene/Scene.hpp"
#include "Graphics/RenderContext.hpp"
#include "Scene/Transform.hpp"

void UIEditableText::init()
{
	UIText::init();
    setIsEditable(true);
}

void UIEditableText::setBackground(const UIElementConfig& config)
{
    UIText::setBackground(config);
    
    if(mBackground)
    {
        getScene()->removeGameObject(mBackground);
    }

    UIBuilder uiBuilder;

    mBackground = uiBuilder.
        setPosition(getTransform()->getWorldPosition() + Vector2(-config.mTextSize.x/RenderContext::getAspectRatio(), config.mTextSize.y/2.0f)).
        setLayer(mConfig.mLayer).
        setIsAffectedByLayout(false).
        setStyle(&UIStyleManager::getInstance().getOrAddStyle<UIStyleEditableTextBackground>()).
        create<UIPanel>().
        getUIElement<UIPanel>();
}
