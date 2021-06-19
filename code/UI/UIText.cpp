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

		if (!mFontRenderers.empty()) {

			if(text.empty()) {
				FOR_ARRAY(i, mFontRenderers) {
					removeComponent(mFontRenderers[i]);
				}

				mFontRenderers.clear();
			} else {

				// Remove Extra Renderers
				if (mFontRenderers.size() > text.length()) {
					FOR_RANGE(i, text.length(), mString.length()) {
						removeComponent(mFontRenderers[i]);
					}

					std::vector<Renderer*> copyVector = mFontRenderers;
					mFontRenderers.clear();
					std::copy(copyVector.begin(), copyVector.begin() + text.length(), std::back_inserter(mFontRenderers));
				}
			}
		}

		if( ! text.empty()) {
			FOR_RANGE(i, 0, text.length()){
				Renderer* renderer = nullptr;

				char character = text.at(i);
				Vector2 textureCoordinates = UI::getInstance()->getCharTextureCoordinates(character);
				Vector2 textureSize = UI::getInstance()->getFontTileTextureSize();

				if (!mFontRenderers.empty() && i < mString.length()) {
					renderer = mFontRenderers[i];
				} else {
					renderer = NEW(Renderer);
					addComponent<Renderer>(renderer);

					renderer->setMesh(Mesh::getRectangle());
					renderer->setMaterial(UI::getInstance()->getFontMaterial());
					renderer->setLayer(mLayer+1);

					mFontRenderers.push_back(renderer);

					newRenderersCreated = true;
				}

				renderer->setPositionOffset(Vector3(((i * mSize.x) - (mSize.x/2.0f)) / RenderContext::getAspectRatio(), 0, 0));
				renderer->setRegion(textureCoordinates.x, textureCoordinates.y, textureSize.x, textureSize.y);
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