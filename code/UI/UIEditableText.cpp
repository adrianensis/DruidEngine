#include "UI/UIEditableText.hpp"
#include "Graphics/Graphics.hpp"
#include "UI/UIManager.hpp"
#include "Scenes/Scene.hpp"
#include "Scenes/Transform.hpp"

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
