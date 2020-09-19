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
#include "Transform.hpp"

namespace DE {

// ---------------------------------------------------------------------------

Renderer::Renderer() : Component() {
	mMesh = nullptr;
	mMaterial = nullptr;
	mAnimations = nullptr;
	mCurrentAnimation = nullptr;
	mColor = nullptr;

	mPositionOffset = Vector3(0.0, 0.0, 0.0);
	mPositionOffsetDirty = true;

	// texture region
	mRegionPosition = Vector2(0.0, 0.0);
	mRegionSize = Vector2(1.0, 1.0);

	mInvertXAxis = false;

	mLineMode = false;

	mLayer = 0;

	mRenderDistance = 1500;
	mOutOfCamera = false;

	mIsAffectedByProjection = true;

	mChunk = nullptr;
	mIsAlreadyInBatch = false;
}

Renderer::~Renderer() {
	if (mAnimations) {
		FOR_LIST(it, mAnimations->getValues()) {
			Memory::free<Animation>(it.get());
		}

		Memory::free<HashMap<std::string, Animation*>>(mAnimations);
	}

	Memory::free<Array<f32>>(mColor);

	if (mVertices) {
		Memory::free<Array<Vector2>>(mVertices);
	}

}

// ---------------------------------------------------------------------------

void Renderer::init() {
	// TRACE();

	mColor = Memory::allocate<Array<f32>>();
	mColor->init(4);

	setColor(Vector4(0, 0, 0, 1));

	//mPositionOffsetMatrix.identity();

	mVertices = Memory::allocate<Array<Vector2>>();
	mVertices->init(4);

	mVertices->set(0, Vector2(0, 0)); // LEFT TOP VERTEX
	mVertices->set(1, Vector2(0, 0)); // LEFT BOTTOM
	mVertices->set(2, Vector2(0, 0)); // RIGHT BOTTOM
	mVertices->set(3, Vector2(0, 0)); // RIGHT TOP

}

// ---------------------------------------------------------------------------

bool Renderer::hasAnimations() const { return mAnimations && mAnimations->getLength() > 0; } ;

const Animation* Renderer::getCurrentAnimation() const {
	return mCurrentAnimation;
}

// ---------------------------------------------------------------------------

void Renderer::setRegion(f32 u, f32 v, f32 width, f32 height) {
	mRegionPosition.x = u;
	mRegionPosition.y = v;

	mRegionSize.x = width;
	mRegionSize.y = height;
}

// ---------------------------------------------------------------------------

void Renderer::setAnimation(const std::string &name) {
	if (mAnimations) {
		if (mAnimations->contains(name)) {
			mCurrentAnimation = mAnimations->get(name);
		}
	}
};

//----------------------------------------------------------------------

void Renderer::addAnimation(const std::string &name, Animation *animation) {
	if (!mAnimations) {
		mAnimations = Memory::allocate<HashMap<std::string, Animation*>>();
		mAnimations->init();
	}

	mAnimations->set(name, animation);
};

// ---------------------------------------------------------------------------

void Renderer::updateAnimation(Material *material) {
	if (mMaterial) {

		Shader* shader = mMaterial->getShader();

		if (hasAnimations()) {
			const AnimationFrame* frame = mCurrentAnimation->getNextFrame();
			mRegionPosition = frame->getPosition();
			mRegionSize = Vector2(frame->getWidth(), frame->getHeight());
		}
	}
};

// ---------------------------------------------------------------------------

void Renderer::setPositionOffset(Vector3 newPositionOffset) {
	mPositionOffset = newPositionOffset;
	//mPositionOffsetMatrix.translation(mPositionOffset);
	mPositionOffsetDirty = true;
};

// ---------------------------------------------------------------------------

const Matrix4& Renderer::getPositionOffsetMatrix() {
	return mPositionOffsetMatrix;
}

void Renderer::setColor(const Vector4 &color) {
	mColor->set(0, color.x);
	mColor->set(1, color.y);
	mColor->set(2, color.z);
	mColor->set(3, color.w);
};

void Renderer::setChunk(Chunk *chunk) {
	mChunk = chunk;
};
Chunk* Renderer::getChunk() {
	return mChunk;
};

bool Renderer::isAffectedByProjection() {

	if(getGameObject()){
		mIsAffectedByProjection = getGameObject()->getTransform()->isAffectedByProjection();
	}

	return mIsAffectedByProjection;
}

// ---------------------------------------------------------------------------

void Renderer::renderCollider() {

	if (mIsAffectedByProjection) {
		List<Collider*>* colliders = getGameObject()->getComponents<Collider>();

		if (colliders && !colliders->isEmpty()) {
			Array<Vector2>* box = colliders->get(0)->getBoundingBox(true);

			f32 lineSize = 1;

			RenderEngine::getInstance()->drawLine(Vector3(box->get(0)), Vector3(box->get(1)), lineSize, mIsAffectedByProjection);
			RenderEngine::getInstance()->drawLine(Vector3(box->get(1)), Vector3(box->get(2)), lineSize, mIsAffectedByProjection);
			RenderEngine::getInstance()->drawLine(Vector3(box->get(2)), Vector3(box->get(3)), lineSize, mIsAffectedByProjection);
			RenderEngine::getInstance()->drawLine(Vector3(box->get(3)), Vector3(box->get(0)), lineSize, mIsAffectedByProjection);
		}
	}
}

// ---------------------------------------------------------------------------

const Matrix4& Renderer::getRendererModelMatrix() {
	if (mPositionOffsetDirty || !isStatic()) {

		mRenderereModelMatrix.translation(mPositionOffset);

		mRenderereModelMatrix.mul(getGameObject()->getTransform()->getModelMatrix());
		mPositionOffsetDirty = false;
	}

	return mRenderereModelMatrix;
};

Array<Vector2>* Renderer::getVertices() {

	if(mPositionOffsetDirty || !isStatic()){
		FOR_ARRAY(i, mVertices) {
			Vector3 vertexPosition(
			mMesh->getVertices()->get(i*3 + 0),
			mMesh->getVertices()->get(i*3 + 1),
			mMesh->getVertices()->get(i*3 + 2));

			vertexPosition = getRendererModelMatrix().mulVector(Vector4(vertexPosition,1));

			mVertices->set(i, vertexPosition);
		}
	}

	return mVertices;
}


} /* namespace DE */
