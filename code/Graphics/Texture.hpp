#pragma once

#include "Core/Core.hpp"
#include <string>

CLASS(Texture, ObjectBase) {

	PRI(TextureId, GET, u32)
	PRI(Data, GET, byte*)
	PRI(Width, GET, u32)
	PRI(Height, GET, u32)
	PRI(Path, GETREF_CONST, std::string)

	byte* readPNG();

public:

	Texture();
	~Texture() override;

	void init(const std::string& path);
	void bind();
};