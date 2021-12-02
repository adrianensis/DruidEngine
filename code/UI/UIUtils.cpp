#include "UI/UIUtils.hpp"
#include "Graphics/Graphics.hpp"

Vector2 UIUtils::correctAspectRatio_X(const Vector2& vector)
{
	Vector2 correctedVector = vector;
	correctedVector.x = correctedVector.x / RenderContext::getAspectRatio();
	return correctedVector;
}