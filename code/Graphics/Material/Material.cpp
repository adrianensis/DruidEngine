#include "Graphics/Material/Material.hpp"

#include "Graphics/Material/Texture.hpp"
#include "Graphics/Material/Shader.hpp"
#include "Graphics/Camera/Camera.hpp"
#include "Graphics/RenderContext.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Core/Time/TimeUtils.hpp"
#include "Profiler/Profiler.hpp"

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
	if (mTexture)
	{
		mTexture->bind();
	}

	Camera *camera = RenderEngine::getInstance().getCamera();

	const Matrix4 &projectionMatrix = camera->getProjectionMatrix();
	const Matrix4 &viewMatrix = camera->getViewMatrix();

	mShader->addMatrix(isWorldSpace ? projectionMatrix : Matrix4::getIdentity(), "projectionMatrix");
	mShader->addMatrix(isWorldSpace ? viewMatrix : Matrix4::getIdentity(), "viewMatrix");

	mShader->addBool(mTexture != nullptr, "hasTexture");
	mShader->addBool(mAlphaEnabled, "alphaEnabled");
	mShader->addBool(mHasBorder, "hasBorder");

	mShader->addFloat(Time::getInstance().getDeltaTimeSeconds(), "time");

	mShader->addVector2(RenderContext::getWindowSize(), "windowSize");
}

void Material::enable()
{
	mShader->enable();
};

void Material::disable()
{
	mShader->disable();
};

void Material::serialize(JSON &json) const
{
	//DO_SERIALIZE("texture", mTexture)
}

void Material::deserialize(const JSON &json)
{
	//DO_DESERIALIZE("texture", mTexture)
}