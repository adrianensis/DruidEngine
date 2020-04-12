#include "CustomScript.h"
#include "Log.h"

#include "GameObject.h"
#include "Scene.h"
#include "Transform.h"
#include "Camera.h"
#include "Renderer.h"
#include "Shader.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Time.h"
#include "Input.h"
#include "List.h"
#include "HashMap.h"

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "MaterialManager.h"
#include "Animation.h"

#include "Collider.h"
#include "RigidBody.h"

#include "Scene.h"
#include "RenderEngine.h"
#include "RenderContext.h"

#include "File.h"
#include "UI.h"
#include "UIButton.h"
#include "UIText.h"
#include "PhysicsEngine.h"

#include "ScenesManager.h"

namespace DE {

// ---------------------------------------------------------------------------

CustomScript::CustomScript() : Script(){

}

// ---------------------------------------------------------------------------

CustomScript::~CustomScript() = default;

// ---------------------------------------------------------------------------

void CustomScript::init(){

}

// ---------------------------------------------------------------------------

void CustomScript::firstStep(){

}

// ---------------------------------------------------------------------------

void CustomScript::step(){
  if(Input::isKeyPressedOnce(GLFW_KEY_SPACE)){
    ScenesManager* sceneManager = ScenesManager::getInstance();
    sceneManager->setScene(1);
  } else if(Input::isKeyPressedOnce(GLFW_KEY_M)){
    ECHO("HEY")
  }
}

// ---------------------------------------------------------------------------

void CustomScript::terminate(){

}

// ---------------------------------------------------------------------------

} /* namespace DE */
