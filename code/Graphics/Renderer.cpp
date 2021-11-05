#include "Graphics/Renderer.hpp"

#include "Graphics/Animation/AnimationFrame.hpp"
#include "Graphics/Material/Shader.hpp"
#include "Log/Log.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Config/ConfigObject.hpp"
#include "Graphics/Material/Texture.hpp"
#include "Graphics/Material/MaterialManager.hpp"
#include "Graphics/Material/Material.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Animation/Animation.hpp"
#include "Graphics/Optimizations/Chunk.hpp"

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
	//MAP_DELETE_CONTENT(mAnimations)
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
		mCurrentAnimation = &mAnimations[name];
	}
};

//----------------------------------------------------------------------

/**
 * Add an animation, by name.
 * \param string name The name.
 * \param Animation animation The animation.
 */
void Renderer::addAnimation(const std::string &name, const Animation& animation)
{
	Animation animationCopy = animation;
    animationCopy.setName(name);
	MAP_INSERT(mAnimations, name, animationCopy);
};

void Renderer::removeAnimation(const std::string &name)
{
    if (MAP_CONTAINS(mAnimations, name))
	{
		if(mCurrentAnimation == &mAnimations[name])
        {
            mCurrentAnimation = nullptr;
        }
	}

    mAnimations.erase(name);
}

void Renderer::updateAnimation()
{
	if (mMaterial)
	{
		if (hasAnimations() && mCurrentAnimation && !mCurrentAnimation->getFrames().empty())
		{
			const AnimationFrame& frame = mCurrentAnimation->getNextFrame();
			mRegion.setLeftTop(frame.getPosition());
			mRegion.setSize(Vector2(frame.getWidth(), frame.getHeight()));
		}
	}
};

void Renderer::setPositionOffset(const Vector3 &newPositionOffset)
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

bool Renderer::hasClipRectangle() const 
{
	return mClipRectangle.getSize().len() > MathUtils::FLOAT_EPSILON;
};

SERIALIZE_IMPL(Renderer)
{
	Component::serialize(json);

	std::string materialPath = "";

	if(mMaterial->getTexture())
	{
		materialPath = mMaterial->getTexture()->getPath();
	}

	DO_SERIALIZE("material", materialPath)
	DO_SERIALIZE("region", mRegion)
	DO_SERIALIZE("layer", mLayer)

    std::list<Animation> tmpList;
    FOR_MAP(it, mAnimations)
    {
        tmpList.push_back(it->second);
    }

    DO_SERIALIZE_LIST("animations", tmpList)
}

DESERIALIZE_IMPL(Renderer)
{
	std::string materialPath = "";
	DO_DESERIALIZE("material", materialPath)

	mMaterial = MaterialManager::getInstance().loadMaterial(materialPath);

	DO_DESERIALIZE("region", mRegion)
	DO_DESERIALIZE("layer", mLayer)

	mMesh = Mesh::getRectangle();

    std::list<Animation> tmpList;
    DO_DESERIALIZE_LIST("animations", tmpList, [](const JSON &json)
    {
        Animation animation;
        animation.deserialize(json);
        return animation;
    });

    FOR_LIST(it, tmpList)
    {
        MAP_INSERT(mAnimations, (*it).getName(), *it)
    }
}