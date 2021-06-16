#include "UI/UIText.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Material/Material.hpp"
#include "Maths/Vector3.hpp"
#include "UI/UI.hpp"
#include "Scene/Scene.hpp"
#include "Graphics/RenderContext.hpp"
#include "Scene/Transform.hpp"

void UIText::init() {
	UIElement::init();
}

void UIText::onDestroy() {
	UIElement::onDestroy();
}

void UIText::setText(const std::string& text) {

	if (mString != text) {

		bool newRenderersCreated = false;

		const std::list<Renderer*>* renderers = getComponents<Renderer>();

		if(text == "") {
			if (renderers && renderers->size() > 0) {
				FOR_LIST(it, *renderers) {
					removeComponent(*it);
				}
			}
		} else {
			// Remove Extra Renderers
			if (renderers && renderers->size() > text.length()) {
				FOR_RANGE(i, text.length(), mString.length()) {
					removeComponent(renderers->back());
				}
			}

			FOR_RANGE(i, 0, text.length()){
				Renderer* renderer = nullptr;

				char character = text.at(i);
				if(character != '\0'){
					Vector2 textureCoordinates = UI::getInstance()->getCharTextureCoordinates(character);
					Vector2 textureSize = UI::getInstance()->getFontTileTextureSize();

					if (renderers && !renderers->empty() && i < mString.length()) {
						auto it = renderers->begin();
						std::advance(it, i);
						renderer = *it;
					} else {
						renderer = new Renderer;
						addComponent<Renderer>(renderer);

						renderer->setMesh(Mesh::getRectangle());
						renderer->setMaterial(UI::getInstance()->getFontMaterial());
						renderer->setLayer(mLayer+1);

						newRenderersCreated = true;
					}

					renderer->setPositionOffset(Vector3(((i * mSize.x) - (mSize.x/2.0f)) / RenderContext::getAspectRatio(), 0, 0));
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