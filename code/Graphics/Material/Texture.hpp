#pragma once

#include "Core/Core.hpp"

class Texture: public ObjectBase
{
    GENERATE_METADATA(Texture)
	PRI u32 mTextureId = {}; GET(TextureId)
	PRI byte * mData = {}; GET(Data)
	PRI u32 mWidth = {}; GET(Width)
	PRI u32 mHeight = {}; GET(Height)
	PRI std::string mPath = {}; GETREF_CONST(Path)

	byte *readPNG();

PUB
	Texture();
	~Texture() override;

	void init(const std::string &path);
	void bind();

	SERIALIZE();
	DESERIALIZE();
};