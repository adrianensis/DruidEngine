#include "Renderer.hpp"

#include "Mesh.hpp"
#include "Animation.hpp"
#include "AnimationFrame.hpp"
#include "Material.hpp"
#include "HashMap.hpp"
#include "Shader.hpp"
#include "Array.hpp"
#include "List.hpp"
#include "Chunk.hpp"
#include "Log.hpp"
#include "RenderEngine.hpp"
#include "Collider.hpp"
#include "GameObject.hpp"

namespace DE {

// ---------------------------------------------------------------------------

Renderer::Renderer() : Component(){
  mMesh = nullptr;
  mMaterial = nullptr;
  mAnimations = nullptr;
  mCurrentAnimation = nullptr;
  mColor = nullptr;

  mPositionOffset = Vector3(0.0,0.0,0.0);

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

  mChunk = nullptr;
  mIsAlreadyInBatch = false;
}

Renderer::~Renderer(){
  if(mAnimations){
    FOR_LIST(it, mAnimations->getValues()){
      Memory::free<Animation>(it.get());
    }

    Memory::free<HashMap<std::string, Animation*>>(mAnimations);
  }

  Memory::free<Array<f32>>(mColor);
}

// ---------------------------------------------------------------------------

void Renderer::init(){
	// TRACE();

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
    if(mAnimations->contains(name)){
      mCurrentAnimation = mAnimations->get(name);
    }
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

void Renderer::updateMaterial(Material* material){
  if(mMaterial->getTexture()){

    Shader* shader = mMaterial->getShader();

    shader->addUInt(mInvertXAxis, "invertXAxis");
    shader->addUInt(mAlphaEnabled && !mLineMode, "alphaEnabled");

    shader->addVector4(mColor,"color");

    if(mAnimations && mAnimations->getLength() > 0){
      const AnimationFrame* frame = mCurrentAnimation->getNextFrame();

      shader->addFloat(frame->getPosition().x, "regionX");
    	shader->addFloat(frame->getPosition().y, "regionY");
    	shader->addFloat(frame->getWidth(), "regionWidth");
    	shader->addFloat(frame->getHeight(), "regionHeight");
      shader->addUInt(mCurrentAnimation->getNumberOfFrames(), "animationSize");

    } else {
      shader->addFloat(mRegionPosition.x, "regionX");
      shader->addFloat(mRegionPosition.y, "regionY");
      shader->addFloat(mRegionSize.x, "regionWidth");
      shader->addFloat(mRegionSize.y, "regionHeight");
      shader->addFloat(1.0f, "animationSize");
    }
  }
};

// ---------------------------------------------------------------------------

void Renderer::setPositionOffset(Vector3 newPositionOffset ){
  mPositionOffset = newPositionOffset;
  mPositionOffsetMatrix.translation(mPositionOffset);
};

// ---------------------------------------------------------------------------

const Matrix4& Renderer::getPositionOffsetMatrix(){
  return mPositionOffsetMatrix;
}

void Renderer::setColor(const Vector4& color){ mColor->set(0,color.x); mColor->set(1,color.y); mColor->set(2,color.z); mColor->set(3,color.w); };
bool Renderer::isLineMode(){ return mLineMode; }

void Renderer::setChunk(Chunk* chunk ){ mChunk = chunk; };
Chunk* Renderer::getChunk(){ return mChunk; };

// ---------------------------------------------------------------------------

void Renderer::renderCollider() {

  if(mIsAffectedByProjection) {
    List<Collider*>* colliders = getGameObject()->getComponents<Collider>();

    if(colliders && ! colliders->isEmpty()) {
      Array<Vector2>* box = colliders->get(0)->getBoundingBox();

      RenderEngine::getInstance()->drawLine(Vector3(box->get(0)), Vector3(box->get(1)), 1, mIsAffectedByProjection);
      RenderEngine::getInstance()->drawLine(Vector3(box->get(1)), Vector3(box->get(2)), 1, mIsAffectedByProjection);
      RenderEngine::getInstance()->drawLine(Vector3(box->get(2)), Vector3(box->get(3)), 1, mIsAffectedByProjection);
      RenderEngine::getInstance()->drawLine(Vector3(box->get(3)), Vector3(box->get(0)), 1, mIsAffectedByProjection);
    }
  }
}

// ---------------------------------------------------------------------------

} /* namespace DE */
