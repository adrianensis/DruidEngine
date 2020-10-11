#include "UIText.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"
#include "Material.hpp"
#include "Vector3.hpp"
#include "UI.hpp"
#include "Scene.hpp"
#include "RenderContext.hpp"
#include "Transform.hpp"

namespace DE {

// ---------------------------------------------------------------------------

UIText::UIText() : UIElement() {
	mString = "";
}

// ---------------------------------------------------------------------------

UIText::~UIText() = default;

// ---------------------------------------------------------------------------


void UIText::init() {
	UIElement::init();
}


void UIText::setText(const std::string &text) {

	if (mString != text) {

		bool newRenderersCreated = false;

		List<Renderer*>* renderers = getComponents<Renderer>();

		if(text == "") {
			if (renderers && renderers->getLength() > 0) {
				FOR_LIST(it, renderers) {
					removeComponent(it.get());
				}
			}
		} else {
			// Remove Extra Renderers
			if (renderers && renderers->getLength() > text.length()) {
				FOR_RANGE(i, text.length(), mString.length()) {
					removeComponent(renderers->getLast().get());
				}
			}

			FOR_RANGE(i, 0, text.length()){
				Renderer* renderer = nullptr;

				c8 character = text.at(i);
				if(character != '\0'){
					Vector2 textureCoordinates = UI::getInstance()->getCharTextureCoordinates(character);
					Vector2 textureSize = UI::getInstance()->getFontTileTextureSize();

					if (renderers && !renderers->isEmpty() && i < mString.length()) {
						renderer = renderers->get(i);
					} else {
						renderer = Memory::allocate<Renderer>();
						addComponent<Renderer>(renderer);

						renderer->setMesh(Mesh::getRectangle());
						renderer->setMaterial(UI::getInstance()->getFontMaterial());
						renderer->setLayer(mLayer+1);

						newRenderersCreated = true;
					}

					renderer->setPositionOffset(Vector3(i * (mSize.x) / RenderContext::getAspectRatio(), 0, 0));
					renderer->setRegion(textureCoordinates.x, textureCoordinates.y, textureSize.x, textureSize.y);
				}
			}
		}

		if (newRenderersCreated) {
			getScene()->updateComponents(this);
		}

		mString = text;

		// NOTE: IMPORTANT - REFRESH COMPONENTS
		setComponentsCache();
	}
}

// ---------------------------------------------------------------------------

} /* namespace DE */
