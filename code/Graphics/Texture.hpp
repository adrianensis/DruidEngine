#pragma once

#include "Core/DE_Class.hpp"
#include <string>

namespace DE {

class Texture: public DE_Class {

	DE_M_GET(TextureId, u32)
	DE_M_GET(Data, byte*)
	DE_M_GET(Width, int)
	DE_M_GET(Height, int)
	DE_M_GET(Path, std::string)

public:

	DE_CLASS(Texture)

	void init(const std::string &path);
};

}

