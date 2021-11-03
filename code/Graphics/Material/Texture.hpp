#pragma once

#include "Core/Core.hpp"

class Texture: public ObjectBase
{
    GENERATE_METADATA(Texture)
	PRI_M(u32, TextureId, GET)
	PRI_M(byte *, Data, GET)
	PRI_M(u32, Width, GET)
	PRI_M(u32, Height, GET)
	PRI_M(SStr, Path, GETREF_CONST)

	byte *readPNG();

PUB
	Texture();
	~Texture() override;

	void init(const SStr &path);
	void bind();

	SERIALIZE();
	DESERIALIZE();
};