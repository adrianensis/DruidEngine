#include "Renderer.h"

#include "Mesh.h"
#include "Animation.h"
#include "AnimationFrame.h"
#include "Material.h"
#include "HashMap.h"
#include "Shader.h"
#include "Array.h"
#include "Log.h"

namespace DE {

// ---------------------------------------------------------------------------

Renderer::Renderer() : Component() {
  mMesh = nullptr;
  mMaterial = nullptr;
  mAnimations = nullptr;
  mCurrentAnimation = nullptr;
  mColor = nullptr;

  // texture region
  mRegionPosition = Vector2(0.0,0.0);
  mRegionWidth = 1.0;
  mRegionHeight = 1.0;

  mInvertXAxis = false;
  mAlphaEnabled = true;
  mLineMode = false;
}

Renderer::~Renderer() {
  Memory::free<HashMap<std::string, Animation*>>(mAnimations);
}

// ---------------------------------------------------------------------------

void Renderer::init() {
	TRACE();

  mAnimations = Memory::allocate<HashMap<std::string, Animation*>>();
  mAnimations->init();

  mColor = Memory::allocate<Array<f32>>();
  mColor->init(4);

  setColor(Vector4(0,0,0,1));
}

// ---------------------------------------------------------------------------

void Renderer::setRegion(f32 u, f32 v, f32 width, f32 height)
{
  mRegionPosition.x = u;
  mRegionPosition.y = v;

  mRegionWidth = width;
  mRegionHeight = height;
}

// ---------------------------------------------------------------------------

/**
* Set the animation, by name.
* @param {String} name The name.
*/
void Renderer::setAnimation(const std::string& name){
	mCurrentAnimation = mAnimations->get(name);
};

//----------------------------------------------------------------------

/**
* Add an animation, by name.
* @param {String} name The name.
* @param {Animation} animation The animation.
*/
void Renderer::addAnimation(const std::string& name, Animation* animation){
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

    shader->addUInt(mInvertXAxis, "invertXAxis");
    shader->addUInt(mAlphaEnabled && !mLineMode, "alphaEnabled");

    shader->addVector4(mColor,"color");

    shader->addFloat(mRegionPosition.x, "regionX");
    shader->addFloat(mRegionPosition.y, "regionY");
    shader->addFloat(mRegionWidth, "regionWidth");
    shader->addFloat(mRegionHeight, "regionHeight");

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

void Renderer::setColor(const Vector4& color) { mColor->set(0,color.x); mColor->set(1,color.y); mColor->set(2,color.z); mColor->set(3,color.w); };
bool Renderer::isLineMode() { return mLineMode; }

// ---------------------------------------------------------------------------

} /* namespace DE */
