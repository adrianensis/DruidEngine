#pragma once

#include "Core/Core.hpp"

CLASS(Texture, ObjectBase)
{
	PRI(u32, TextureId, GET)
	PRI(byte *, Data, GET)
	PRI(u32, Width, GET)
	PRI(u32, Height, GET)
	PRI(SStr, Path, GETREF_CONST)

	byte *readPNG();

public:
	Texture();
	~Texture() override;

	void init(const SStr &path);
	void bind();

	SERIALIZE();
	DESERIALIZE();
};