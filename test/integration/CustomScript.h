#ifndef DE_CUSTOMSCRIPT_H
#define DE_CUSTOMSCRIPT_H

#include "Script.h"
#include "Vector2.h"
#include "PlayerScript.h"

namespace DE {

class Renderer;
class Transform;
class RigidBody;
class Collider;
class GameObject;
class Camera;
class UIButton;
class UIText;
class Material;
template <class K, class V> class HashMap;

class CustomScript : public Script{
private:

public:

DE_CLASS(CustomScript, Script);

void init() override;
void firstStep() override;
void step() override;
void terminate() override;

};

} /* namespace DE */

#endif /* DE_CUSTOMSCRIPT_H */
