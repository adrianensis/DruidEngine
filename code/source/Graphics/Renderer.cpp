#include "Renderer.h"

#include "Mesh.h"
#include "Animation.h"
#include "AnimationFrame.h"
#include "Material.h"
#include "HashMap.h"
#include "Shader.h"
#include "Array.h"
#include "Chunk.h"
#include "Log.h"

namespace DE {

// ---------------------------------------------------------------------------

Renderer::Renderer() : Component() {
  mMesh = nullptr;
  mMaterial = nullptr;
  mAnimations = nullptr;
  mCurrentAnimation = nullptr;
  mColor = nullptr;

  mPositionOffset = Vector3(0.0,0.0,0.0);
  mPositionOffsetMatrix = nullptr;

  // texture region
  mRegionPosition = Vector2(0.0,0.0);
  mRegionWidth = 1.0;
  mRegionHeight = 1.0;

  mInvertXAxis = false;
  mAlphaEnabled = true;
  mLineMode = false;

  mLayer = 0;

  mRenderDistance = 1500;
  mOutOfCamera = false;

}

Renderer::~Renderer() {
  Memory::free<HashMap<std::string, Animation*>>(mAnimations);
  Memory::free<Array<f32>>(mColor);

  if(mPositionOffsetMatrix)
    Memory::free<Matrix4>(mPositionOffsetMatrix);
}

// ---------------------------------------------------------------------------

void Renderer::init() {
	// TRACE();

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

void Renderer::setAnimation(const std::string& name){
	mCurrentAnimation = mAnimations->get(name);
};

//----------------------------------------------------------------------

void Renderer::addAnimation(const std::string& name, Animation* animation){
	mAnimations->set(name, animation);
};

// ---------------------------------------------------------------------------

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

const Matrix4& Renderer::getPositionOffsetMatrix(){
  if(! mPositionOffsetMatrix){
    mPositionOffsetMatrix = Memory::allocate<Matrix4>();

  }

  mPositionOffsetMatrix->translation(mPositionOffset);

  return *mPositionOffsetMatrix;
}


void Renderer::setColor(const Vector4& color) { mColor->set(0,color.x); mColor->set(1,color.y); mColor->set(2,color.z); mColor->set(3,color.w); };
bool Renderer::isLineMode() { return mLineMode; }

void Renderer::setChunk(Chunk* chunk ) { mChunk = chunk; };
Chunk* Renderer::getChunk() { return mChunk; };

// ---------------------------------------------------------------------------

} /* namespace DE */
