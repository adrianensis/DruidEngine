#ifndef DE_UI_H
#define DE_UI_H

#include "DE_Class.h"
#include "Vector2.h"

namespace DE {

class GameObject;
class Texture;
class Material;

class UI : public DE_Class {
private:

static Texture* smTexture;
static Material* smMaterial;

public:

DE_CLASS(UI, DE_Class);

static GameObject* createButton(const Vector2& position, const Vector2& size);

};

} /* namespace DE */

#endif /* DE_UI_H */
