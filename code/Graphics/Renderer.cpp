#include "Graphics/Renderer.hpp"

#include "Graphics/Animation/AnimationFrame.hpp"
#include "Graphics/Shader.hpp"
#include "Log/Log.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Config/ConfigObject.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/MaterialManager.hpp"

Renderer::Renderer() {
	mMesh = nullptr;
	mMaterial = nullptr;
	mCurrentAnimation = nullptr;

	mPositionOffset = Vector3(0.0, 0.0, 0.0);
	mPositionOffsetDirty = true;

	// texture region
	mRegionPosition = Vector2(0.0, 0.0);
	mRegionSize = Vector2(1.0, 1.0);

	mIsInvertAxis = false;

	mIsLineMode = false;

	mLayer = 0;

	mRenderDistance = 1500; // TODO : move to settings?
	mIsOutOfCamera = false;

	mIsAffectedByProjection = true;

	mChunk = nullptr;
	mIsAlreadyInBatch = false;

	mForceRecalculateVertices = false;
}

Renderer::~Renderer() {
	FOR_MAP(it, mAnimations) {
		delete it->second;
	}
}

void Renderer::init() {
	// TRACE();

	setColor(Vector4(0, 0, 0, 1));

	mVertices.reserve(4);

	mVertices[0] = Vector2(0, 0); // LEFT TOP VERTEX
	mVertices[1] = Vector2(0, 0); // LEFT BOTTOM
	mVertices[2] = Vector2(0, 0); // RIGHT BOTTOM
	mVertices[3] = Vector2(0, 0); // RIGHT TOP

	mForceRecalculateVertices = false;

}

bool Renderer::hasAnimations() const { return mAnimations.size() > 0; } ;

void Renderer::setRegion(f32 u, f32 v, f32 width, f32 height) {
	mRegionPosition.x = u;
	mRegionPosition.y = v;

	mRegionSize.x = width;
	mRegionSize.y = height;
}

/**
 * Set the animation, by name.
 * \param string name The name.
 */
void Renderer::setAnimation(const std::string& name) {
	if (MAP_CONTAINS(mAnimations, name)) {
			mCurrentAnimation = mAnimations[name];
		}
};

//----------------------------------------------------------------------

/**
 * Add an animation, by name.
 * \param string name The name.
 * \param Animation animation The animation.
 */
void Renderer::addAnimation(const std::string& name, Animation *animation) {
	MAP_INSERT(mAnimations, name, animation);
};

void Renderer::updateAnimation() {
	if (mMaterial) {
		if (hasAnimations()) {
			const AnimationFrame* frame = mCurrentAnimation->getNextFrame();
			mRegionPosition = frame->getPosition();
			mRegionSize = Vector2(frame->getWidth(), frame->getHeight());
		}
	}
};

void Renderer::setPositionOffset(Vector3 newPositionOffset) {
	mPositionOffset = newPositionOffset;
	mPositionOffsetDirty = true;
};

void Renderer::setColor(const Vector4 &color) {
	mColor[0] = color.x;
	mColor[1] = color.y;
	mColor[2] = color.z;
	mColor[3] = color.w;
};

bool Renderer::getIsAffectedByProjection() {

	if(getGameObject()){
		mIsAffectedByProjection = getGameObject()->getTransform()->getAffectedByProjection();
	}

	return mIsAffectedByProjection;
}

const std::vector<Vector2>* Renderer::getVertices(bool force /*= false*/) {

	if(mPositionOffsetDirty || !isStatic() || force || mForceRecalculateVertices ){

		mRenderereModelMatrix.translation(mPositionOffset);

		mRenderereModelMatrix.mul(getGameObject()->getTransform()->getModelMatrix());
		mPositionOffsetDirty = false;

		FOR_ARRAY(i, mVertices) {
			Vector3 vertexPosition(
			mMesh->getVertices()[i*3 + 0],
			mMesh->getVertices()[i*3 + 1],
			mMesh->getVertices()[i*3 + 2]);

			vertexPosition = mRenderereModelMatrix.mulVector(Vector4(vertexPosition,1));

			mVertices[i] = vertexPosition;
		}

		mForceRecalculateVertices = false;
	}

	return &mVertices;
}

void Renderer::forceRecalculateVertices() {
	getGameObject()->getTransform()->forceModelMatrixCalculation();
	mForceRecalculateVertices = true;
}

/*void Renderer::serialize(ConfigObject* configMap, std::string& objectName) {
	Texture *texture = getMaterial()->getTexture();
	configMap->setString(objectName + ".texture.path", texture->getPath());
	configMap->setF32(objectName + ".texture.region.u", getRegionPosition().x);
	configMap->setF32(objectName + ".texture.region.v", getRegionPosition().y);
	configMap->setF32(objectName + ".texture.region.width", getRegionSize().x);
	configMap->setF32(objectName + ".texture.region.height", getRegionSize().y);
	configMap->setU32(objectName + ".layer", getLayer());
}

void Renderer::deserialize(ConfigObject* configMap, std::string& objectName) {
	Material* material = MaterialManager::getInstance()->loadMaterial(configMap->getString(objectName + ".texture.path"));
	Vector2 textureRegionPosition(configMap->getF32(objectName + ".texture.region.u"),
			configMap->getF32(objectName + ".texture.region.v"));
	Vector2 textureRegionSize(configMap->getF32(objectName + ".texture.region.width"),
			configMap->getF32(objectName + ".texture.region.height"));

	setLayer(configMap->getU32(objectName + ".layer"));
	setMesh(Mesh::getRectangle());
	setMaterial(material);
	setRegion(textureRegionPosition.x, textureRegionPosition.y, textureRegionSize.x, textureRegionSize.y);

}*/