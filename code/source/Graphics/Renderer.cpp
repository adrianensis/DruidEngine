#include "Renderer.h"

#include "Mesh.h"
#include "Animation.h"
#include "AnimationFrame.h"
#include "Material.h"
#include "HashMap.h"
#include "Shader.h"

namespace DE {

// ---------------------------------------------------------------------------

Renderer::Renderer() : Component() {
  mMesh = nullptr;
  mMaterial = nullptr;
  mAnimations = nullptr;
  mCurrentAnimation = nullptr;
}

Renderer::~Renderer() {
  Memory::free<HashMap<std::string, Animation*>>(mAnimations);
}

// ---------------------------------------------------------------------------

void Renderer::init() {
  mAnimations = Memory::allocate<HashMap<std::string, Animation*>>();
  mAnimations->init();
}

// ---------------------------------------------------------------------------

/**
* Set the animation, by name.
* @param {String} name The name.
*/
void Renderer::setAnimation(std::string name){
	mCurrentAnimation = mAnimations->get(name);
};

//----------------------------------------------------------------------

/**
* Add an animation, by name.
* @param {String} name The name.
* @param {Animation} animation The animation.
*/
void Renderer::addAnimation(std::string name, Animation* animation){
	mAnimations->set(name, animation);
};

// ---------------------------------------------------------------------------

/**
* Update de material with information about the region, the alphacolor and the animation.
* @param {Material} material The material.
*/
void Renderer::updateMaterial(Material* material) {
  if(mMaterial->getTexture()) {

    Shader* shader = mMaterial->getShader();

    //shader.addFloatVector(this.alphaColor.toArray(),"alphacolor");

    // shader->addFloat(this.getRegionPosition().x, "regionX");
    // shader->addFloat(this.getRegionPosition().y, "regionY");
    // shader->addFloat(this.getRegionWidth(), "regionWidth");
    // shader->addFloat(this.getRegionHeight(), "regionHeight");

    if(mAnimations->getLength() > 0) {
      const AnimationFrame* frame = mCurrentAnimation->getNextFrame();

      shader->addFloat(frame->getPosition().x, "animationX");
    	shader->addFloat(frame->getPosition().y, "animationY");
    	shader->addFloat(frame->getWidth(), "animationWidth");
    	shader->addFloat(frame->getHeight(), "animationHeight");

    } else {
      shader->addFloat(0.0f, "animationX");
    	shader->addFloat(0.0f, "animationY");
    	shader->addFloat(1.0f, "animationWidth");
    	shader->addFloat(1.0f, "animationHeight");
    }
  }
};

// ---------------------------------------------------------------------------

void Renderer::setMesh(Mesh* mesh){ mMesh = mesh; }
Mesh* Renderer::getMesh(){ return mMesh; }
void Renderer::setMaterial(Material* material){ mMaterial = material; }
Material* Renderer::getMaterial(){ return mMaterial; }

// ---------------------------------------------------------------------------

} /* namespace DE */
