#include "Graphics/Material/Material.hpp"

#include "Graphics/Material/Texture.hpp"
#include "Graphics/Material/Shader.hpp"
#include "Graphics/Camera/Camera.hpp"
#include "Graphics/RenderContext.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Core/Time/TimeUtils.hpp"

Material::Material()
{
	mShader = nullptr;
	mTexture = nullptr;
	mAlphaEnabled = true;
	mHasBorder = false;
}

void Material::init()
{
}

void Material::bind(bool isWorldSpace)
{
	mShader->use();

	if (mTexture)
	{
		mTexture->bind();
	}

	Camera *camera = RenderEngine::getInstance()->getCamera();

	CNS Matrix4 &projectionMatrix = camera->getProjectionMatrix();
	CNS Matrix4 &viewTranslationMatrix = camera->getViewTranslationMatrix();
	CNS Matrix4 &viewRotationMatrix = camera->getViewRotationMatrix();

	mShader->addMatrix(projectionMatrix, "projectionMatrix");
	mShader->addMatrix(viewTranslationMatrix, "viewTranslationMatrix");
	mShader->addMatrix(viewRotationMatrix, "viewRotationMatrix");

	mShader->addBool(mTexture != nullptr, "hasTexture");
	mShader->addBool(mAlphaEnabled, "alphaEnabled");
	mShader->addBool(mHasBorder, "hasBorder");

	mShader->addFloat(Time::getInstance()->getDeltaTimeSeconds(), "time");

	mShader->addBool(isWorldSpace, "isWorldSpace");

	mShader->addVector2(RenderContext::getWindowSize(), "windowSize");
}

SERIALIZE_IMPL(Material)
{
	//DO_SERIALIZE("texture", mTexture)
}

void Material::deserialize(CNS JSON &json)
{
	//DO_DESERIALIZE("texture", mTexture)
}