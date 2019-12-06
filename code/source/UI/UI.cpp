#include "UI.h"
#include "Memory.h"
#include "Texture.h"
#include "Material.h"
#include "GameObject.h"
#include "Renderer.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Mesh.h"
#include "Shader.h"
#include "Transform.h"
#include "Input.h"
#include "Camera.h"
#include "RenderEngine.h"
#include "List.h"
#include "UIElement.h"

namespace DE {

// ---------------------------------------------------------------------------

UI::UI() : DE_Class(), Singleton() {
  mTexture = nullptr;
  mMaterial = nullptr;
  mUIElements = nullptr;
}

UI::~UI() = default;

// ---------------------------------------------------------------------------

UIButton* UI::createButton(const Vector2& position, const Vector2& size) {

  if( ! mTexture){
    mTexture = Memory::allocate<Texture>();
    mTexture->init("resources/button.bmp");

    mMaterial = Memory::allocate<Material>();
    mMaterial->init();
    mMaterial->setTexture(mTexture);
    mMaterial->setShader(Shader::getDefaultShader());
  }

  UIButton* button = Memory::allocate<UIButton>();
  button->init();

  button->getTransform()->setLocalPosition(Vector3(400,0,0));
  button->getTransform()->setScale(Vector3(size.x,size.y,1));

  Renderer* renderer = Memory::allocate<Renderer>();
  button->addComponent<Renderer>(renderer);

  renderer->setMesh(Mesh::getRectangle());
  renderer->setMaterial(mMaterial);

  RigidBody* rigidBody = Memory::allocate<RigidBody>();
  button->addComponent<RigidBody>(rigidBody);

  Collider* collider = Memory::allocate<Collider>();
  button->addComponent<Collider>(collider);
  collider->setSize(size.x,size.y);

  mUIElements->pushBack(button);

  return button;
}

// ---------------------------------------------------------------------------

void UI::init() {
  mUIElements = Memory::allocate<List<UIElement*>>();
  mUIElements->init();
}

// ---------------------------------------------------------------------------

void UI::step() {

  if(Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)){

    Vector2 mouse(Input::getMousePosition());
    Vector3 world = RenderEngine::getInstance()->getCamera()->screenToWorld(mouse);

    FOR_LIST(it, mUIElements){

      UIElement* element = it.get();

      if(element->getComponents<Collider>()->get(0)->testPoint(world) == ColliderStatus::STATUS_PENETRATION){

        element->onPressed();
        element->getComponents<Renderer>()->get(0)->setColor(Vector4(0,1,0,1));
      }
    }
  }
}

// ---------------------------------------------------------------------------

void UI::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
