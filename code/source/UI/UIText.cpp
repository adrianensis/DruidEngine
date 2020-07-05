#include "UIText.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "Material.hpp"
#include "Vector3.hpp"
#include "UI.hpp"
#include "Scene.hpp"
#include "RenderContext.hpp"

namespace DE {

// ---------------------------------------------------------------------------

UIText::UIText() :
		UIElement() {
	mString = "";
}

// ---------------------------------------------------------------------------

UIText::~UIText() = default;

// ---------------------------------------------------------------------------

void UIText::setText(const std::string &text) {

	if (mString != text) {

		List<Renderer*>* renderers = getComponents<Renderer>();

		// Remove Extra Renderers
		if (renderers && renderers->getLength() > text.length()) {
			VAR(u32, renderers->getLength())
			VAR(u32, mString.length())
			VAR(u32, text.length())
			FOR_RANGE(i, text.length(), mString.length())
			{
				removeComponent(renderers->getLast().get());
			}
		}

		bool newRenderersCreated = false;

		FOR_RANGE(i, 0, text.length())
		{
			Renderer* renderer = nullptr;

			Vector2 textureCoordinates = UI::getInstance()->getCharTextureCoordinates(text.at(i));
			Vector2 textureSize = UI::getInstance()->getFontTileTextureSize();

			if (renderers && !renderers->isEmpty() && i < mString.length()) {
				renderer = renderers->get(i);
			} else {
				renderer = Memory::allocate<Renderer>();
				addComponent<Renderer>(renderer);

				renderer->setMesh(Mesh::getRectangle());
				renderer->setMaterial(UI::getInstance()->getFontMaterial());
				renderer->setLayer(mLayer);

				renderer->setAffectedByProjection(false);

				newRenderersCreated = true;
			}

			renderer->setPositionOffset(Vector3(i * mSize.x / RenderContext::getAspectRatio(), 0, 0));
			renderer->setRegion(textureCoordinates.x, textureCoordinates.y, textureSize.x, textureSize.y);
		}

		if (newRenderersCreated) {
			getScene()->updateComponents(this);
		}

		mString = text;
	}
}

// ---------------------------------------------------------------------------

} /* namespace DE */
