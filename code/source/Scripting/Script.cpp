#include "Script.h"
#include "Debug.h"

#include "GameObject.h"
#include "Transform.h"
#include "Vector3.h"

namespace DE {

Script::Script() : Component()
{
};

Script::~Script() = default;

void Script::init(){

};

void Script::step(){
  ECHO("CUSTOM SCRIPT STEP")

  getGameObject()->getTransform()->translate(Vector3(0.002f,0,0));
};

void Script::update() {
}

void Script::terminate() {

}

} /* namespace DE */
