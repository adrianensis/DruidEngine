#include "TestUI.h"
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
#include "Array.h"

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Animation.h"

#include "Collider.h"
#include "RigidBody.h"

#include "Scene.h"
#include "RenderEngine.h"
#include "RenderContext.h"

#include "File.h"

namespace DE {

// ---------------------------------------------------------------------------

TestUI::TestUI() : Script(){

}

// ---------------------------------------------------------------------------

TestUI::~TestUI() = default;

// ---------------------------------------------------------------------------

void TestUI::init(){

  mTexture = nullptr;
  mMaterial = nullptr;

  mCamera = nullptr;
  mCameraTransform = nullptr;
}

// ---------------------------------------------------------------------------

void TestUI::step(){

  if(! mCamera){
    mCamera = getGameObject()->getScene()->getCameraGameObject()->getComponents<Camera>()->get(0);
    mCameraTransform = mCamera->getGameObject()->getTransform();
  }

  if(Input::isMouseButtonPressedOnce(GLFW_MOUSE_BUTTON_RIGHT)){

  }

  if(Input::isMouseButtonPressedOnce(GLFW_MOUSE_BUTTON_LEFT)){

    //
    // if(!mTestTile){
    //   //createTile();
    // }

    if(!mTestButton){
      mTestButton = UI::getInstance()->createButton(getGameObject()->getScene(), Vector3(0,500,0), Vector2(200,100));

      mTestButton->setOnPressedCallback([&]() {

      });
    }

    if(!mTestText){
      mTestText = UI::getInstance()->createText(getGameObject()->getScene(), Vector3(-100,300,0), Vector2(40,40), "test text");
    }

    if(!mTestList){
      mTestList = UI::getInstance()->createList(getGameObject()->getScene(), Vector3(-500,200,0), Vector2(400,700));
    }
  }

  f32 movement = 1000.0f * Time::getDeltaTimeSeconds();

  if(Input::isKeyPressedOnce(GLFW_KEY_UP)){
    mCameraTransform->translate(Vector3(0,movement,0));

  }else if(Input::isKeyPressedOnce(GLFW_KEY_DOWN)){
    mCameraTransform->translate(Vector3(0,-movement,0));

  }else if(Input::isKeyPressedOnce(GLFW_KEY_LEFT)){
    mCameraTransform->translate(Vector3(-movement,0,0));

  }else if(Input::isKeyPressedOnce(GLFW_KEY_RIGHT)){
    mCameraTransform->translate(Vector3(movement,0,0));

  }else if(Input::isKeyPressedOnce(GLFW_KEY_S)){

  }

  Vector2 mouse(Input::getMousePosition());

  // VAR(f32, mouse.x);
  // VAR(f32, mouse.y);

  Vector3 world = mCamera->screenToWorld(mouse);

  // VAR(f32, world.x);
  // VAR(f32, world.y);

}

// ---------------------------------------------------------------------------

void TestUI::terminate() {

}

// ---------------------------------------------------------------------------

} /* namespace DE */
