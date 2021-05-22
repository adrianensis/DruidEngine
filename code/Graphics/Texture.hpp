#pragma once

#include "Core/ObjectBase.hpp"
#include <string>

CLASS(Texture, ObjectBase) {

	PRI(TextureId, GET, u32)
	PRI(Data, GET, byte*)
	PRI(Width, GET, int)
	PRI(Height, GET, int)
	PRI(Path, GET, std::string)

public:

	Texture();
	virtual ~Texture() override;

	void init(const std::string& path);
};