#include "Graphics/Renderer.hpp"

#include "Graphics/Animation/AnimationFrame.hpp"
#include "Graphics/Material/Shader.hpp"
#include "Log/Log.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Config/ConfigObject.hpp"
#include "Graphics/Material/Texture.hpp"
#include "Graphics/Material/Material.hpp"
#include "Graphics/Material/MaterialManager.hpp"

Renderer::Renderer()
{
	mMesh = nullptr;
	mMaterial = nullptr;
	mCurrentAnimation = nullptr;

	mPositionOffset = Vector3(0.0, 0.0, 0.0);
	mPositionOffsetDirty = true;

	// texture region
	mRegion.setLeftTop(Vector2(0.0, 0.0));
	mRegion.setSize(Vector2(1.0, 1.0));

	mInvertAxisX = false;

	mIsLineMode = false;

	mLayer = 0;

	mRenderDistance = 1500; // TODO : move to settings?
	mIsOutOfCamera = false;

	mIsWorldSpace = true;

	mChunk = nullptr;
	mIsAlreadyInBatch = false;

	mForceRecalculateVertices = false;
}

Renderer::~Renderer()
{
	MAP_DELETE_CONTENT(mAnimations)
}

void Renderer::init()
{
	// TRACE();

	setColor(Vector4(0, 0, 0, 1));

	mVertices.reserve(4);

	mVertices.push_back(Vector2(0, 0)); // LEFT TOP VERTEX
	mVertices.push_back(Vector2(0, 0)); // LEFT BOTTOM
	mVertices.push_back(Vector2(0, 0)); // RIGHT BOTTOM
	mVertices.push_back(Vector2(0, 0)); // RIGHT TOP

	mForceRecalculateVertices = false;
}

bool Renderer::hasAnimations() const { return mAnimations.size() > 0; };

/**
 * Set the animation, by name.
 * \param string name The name.
 */
void Renderer::setAnimation(const std::string &name)
{
	if (MAP_CONTAINS(mAnimations, name))
	{
		mCurrentAnimation = mAnimations[name];
	}
};

//----------------------------------------------------------------------

/**
 * Add an animation, by name.
 * \param string name The name.
 * \param Animation animation The animation.
 */
void Renderer::addAnimation(const std::string &name, Animation *animation)
{
	MAP_INSERT(mAnimations, name, animation);
};

void Renderer::updateAnimation()
{
	if (mMaterial)
	{
		if (hasAnimations())
		{
			const AnimationFrame *frame = mCurrentAnimation->getNextFrame();
			mRegion.setLeftTop(frame->getPosition());
			mRegion.setSize(Vector2(frame->getWidth(), frame->getHeight()));
		}
	}
};

void Renderer::setPositionOffset(Vector3 newPositionOffset)
{
	mPositionOffset = newPositionOffset;
	mPositionOffsetDirty = true;
};

void Renderer::setColor(const Vector4 &color)
{
	mColor[0] = color.x;
	mColor[1] = color.y;
	mColor[2] = color.z;
	mColor[3] = color.w;
};

bool Renderer::getIsWorldSpace()
{
	if (getGameObject())
	{
		mIsWorldSpace = getGameObject()->getTransform()->getAffectedByProjection();
	}

	return mIsWorldSpace;
}

const std::vector<Vector2> &Renderer::getVertices(bool force /*= false*/)
{
	if (mPositionOffsetDirty || !isStatic() || force || mForceRecalculateVertices)
	{
		mRenderereModelMatrix.translation(mPositionOffset);

		mRenderereModelMatrix.mul(getGameObject()->getTransform()->getModelMatrix());
		mPositionOffsetDirty = false;

		FOR_ARRAY(i, mVertices)
		{
			Vector3 vertexPosition(
				mMesh->getVertices()[i * 3 + 0],
				mMesh->getVertices()[i * 3 + 1],
				mMesh->getVertices()[i * 3 + 2]);

			vertexPosition = mRenderereModelMatrix.mulVector(Vector4(vertexPosition, 1));

			mVertices[i] = vertexPosition;
		}

		mForceRecalculateVertices = false;
	}

	return mVertices;
}

void Renderer::forceRecalculateVertices()
{
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
	Vector2 textureRegionPosition(configMap->at(objectName + ".texture.region.u").get<f32>(),
			configMap->at(objectName + ".texture.region.v").get<f32>());
	Vector2 textureRegionSize(configMap->at(objectName + ".texture.region.width").get<f32>(),
			configMap->at(objectName + ".texture.region.height").get<f32>());

	setLayer(configMap->at(objectName + ".layer").get<u32>());
	setMesh(Mesh::getRectangle());
	setMaterial(material);
	setRegion(textureRegionPosition.x, textureRegionPosition.y, textureRegionSize.x, textureRegionSize.y);

}*/