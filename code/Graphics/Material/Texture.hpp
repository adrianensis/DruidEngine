#pragma once

#include "Core/Core.hpp"
class Texture: public ObjectBase
{
    GENERATE_METADATA(Texture)
	PRI u32 mTextureId = 0; GET(TextureId)
	PRI byte* mData = nullptr; GET(Data)
	PRI u32 mWidth = 0; GET(Width)
	PRI u32 mHeight = 0; GET(Height)
	PRI std::string mPath; GETREF_CONST(Path)

	byte *readPNG();

PUB
	Texture();
	~Texture() override;

	void init(const std::string &path);
	void bind();

	virtual void serialize(JSON &json) const override;
	virtual void deserialize(const JSON &json) override;
};