#pragma once

#include "Core/ObjectBase.hpp"
#include <string>

namespace DE {

class Texture : public ObjectBase {

	 u32 mTextureId;
	 byte* mData;
	 int mWidth;
	 int mHeight;
	 String mPath;

public:

	GENERATE_METADATA(Texture);

	Texture();
	virtual ~Texture() override;;

	GET(TextureId);
	GET(Data);
	GET(Width);
	GET(Height);
	GET(Path);

	void init(const String &path);
};
}

