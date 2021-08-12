#include "UI/UIPanel.hpp"

#include "Graphics/Renderer.hpp"
#include "Scene/Transform.hpp"
#include "UI/UIElementConfig.hpp"
#include "UI/UIStyle.hpp"

void UIPanel::initFromConfig(const UIElementConfig& config)
{
    Super::initFromConfig(config);

    Vector3 size = mConfig.mSize;
	size.z = 1;
	size.x = size.x / RenderContext::getAspectRatio();

	getTransform()->setLocalPosition(mConfig.mDisplayPosition);
	getTransform()->setScale(Vector3(UIUtils::correctAspectRatio_X(mConfig.mSize)));
	getTransform()->setAffectedByProjection(false);

	Renderer *renderer = NEW(Renderer);
	addComponent<Renderer>(renderer);

	renderer->setMesh(Mesh::getRectangle());
	//renderer->setMaterial(mButtonMaterial);
	renderer->setMaterial(mConfig.mMaterial);
	renderer->setColor(mConfig.mStyle->mColor);
	renderer->setLayer(mConfig.mLayer);
	//renderer->setHasBorder(true);

    setComponentsCache();
}