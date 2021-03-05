#pragma once

#include "Core/DE_Class.hpp"
#include <string>

namespace DE {

class Texture: public DE_Class {

	DE_M_GET(TextureId, u32)
	DE_M_GET(Data, byte*)
	DE_M_GET(Width, int)
	DE_M_GET(Height, int)
	DE_M_GET(Path, String)

public:

	DE_CLASS_BODY(Texture)

	void init(const String &path);
};

}

