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
  // mPositionOffsetMatrix = nullptr;

  // texture region
  mRegionPosition = Vector2(0.0,0.0);
  mRegionSize = Vector2(1.0,1.0);

  mInvertXAxis = false;
  mAlphaEnabled = true;
  mLineMode = false;

  mLayer = 0;

  mRenderDistance = 1500;
  mOutOfCamera = false;

  mIsAffectedByProjection = true;
}

Renderer::~Renderer() {
  if(mAnimations){
    Memory::free<HashMap<std::string, Animation*>>(mAnimations);
  }
  Memory::free<Array<f32>>(mColor);

  // if(mPositionOffsetMatrix)
  //   Memory::free<Matrix4>(mPositionOffsetMatrix);
}

// ---------------------------------------------------------------------------

void Renderer::init() {
	// TRACE();

  // mAnimations = Memory::allocate<HashMap<std::string, Animation*>>();
  // mAnimations->init();

  mColor = Memory::allocate<Array<f32>>();
  mColor->init(4);

  setColor(Vector4(0,0,0,1));

  mPositionOffsetMatrix.identity();
}

// ---------------------------------------------------------------------------

void Renderer::setRegion(f32 u, f32 v, f32 width, f32 height){
  mRegionPosition.x = u;
  mRegionPosition.y = v;

  mRegionSize.x = width;
  mRegionSize.y = height;
}

// ---------------------------------------------------------------------------

void Renderer::setAnimation(const std::string& name){
  if(mAnimations){
    mCurrentAnimation = mAnimations->get(name);
  }
};

//----------------------------------------------------------------------

void Renderer::addAnimation(const std::string& name, Animation* animation){
  if(!mAnimations){
    mAnimations = Memory::allocate<HashMap<std::string, Animation*>>();
    mAnimations->init();
  }

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
    shader->addFloat(mRegionSize.x, "regionWidth");
    shader->addFloat(mRegionSize.y, "regionHeight");

    if(mAnimations && mAnimations->getLength() > 0) {
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

void Renderer::setPositionOffset(Vector3 newPositionOffset ) {
  mPositionOffset = newPositionOffset;
  mPositionOffsetMatrix.translation(mPositionOffset);
};

// ---------------------------------------------------------------------------

const Matrix4& Renderer::getPositionOffsetMatrix(){
  return mPositionOffsetMatrix;
}

void Renderer::setColor(const Vector4& color) { mColor->set(0,color.x); mColor->set(1,color.y); mColor->set(2,color.z); mColor->set(3,color.w); };
bool Renderer::isLineMode() { return mLineMode; }

void Renderer::setChunk(Chunk* chunk ) { mChunk = chunk; };
Chunk* Renderer::getChunk() { return mChunk; };

// ---------------------------------------------------------------------------

} /* namespace DE */
