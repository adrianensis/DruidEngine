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

namespace DE {

  Texture* UI::smTexture = nullptr;
  Material* UI::smMaterial = nullptr;

// ---------------------------------------------------------------------------

GameObject* UI::createButton(const Vector2& position, const Vector2& size) {

  if( ! smTexture){
    smTexture = Memory::allocate<Texture>();
    smTexture->init("resources/button.bmp");

    smMaterial = Memory::allocate<Material>();
    smMaterial->init();
    smMaterial->setTexture(smTexture);
    smMaterial->setShader(Shader::getDefaultShader());
  }

  GameObject* button = Memory::allocate<GameObject>();
  button->init();

  button->getTransform()->setLocalPosition(Vector3(400,0,0));
  button->getTransform()->setScale(Vector3(size.x,size.y,1));

  Renderer* renderer = Memory::allocate<Renderer>();
  button->addComponent<Renderer>(renderer);

  renderer->setMesh(Mesh::getRectangle());
  renderer->setMaterial(smMaterial);

  RigidBody* rigidBody = Memory::allocate<RigidBody>();
  button->addComponent<RigidBody>(rigidBody);

  Collider* collider = Memory::allocate<Collider>();
  button->addComponent<Collider>(collider);
  collider->setSize(size.x,size.y);

  return button;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
