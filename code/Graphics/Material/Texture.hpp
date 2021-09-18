#pragma once

#include "Core/Core.hpp"

CLASS(Texture, ObjectBase)
{
	PRI(TextureId, GET, u32)
	PRI(Data, GET, byte *)
	PRI(Width, GET, u32)
	PRI(Height, GET, u32)
	PRI(Path, GETREF_CONST, SStr)

	byte *readPNG();

public:
	Texture();
	~Texture() OVR;

	void init(CNS SStr &path);
	void bind();

	SERIALIZE();
	DESERIALIZE();
};