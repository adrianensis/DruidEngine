#include "Graphics/Texture.hpp"
#include "Log/Log.hpp"
#include "SOIL/SOIL.h"

namespace DE {

Texture::Texture() : DE_Class() {
	mTextureId = -1;
	mData = nullptr;
}

Texture::~Texture() {
	SOIL_free_image_data(mData);
}

void Texture::init(const String &path) {
	DE_TRACE()

	if (!mData) {
		mPath = path;
		mData = SOIL_load_image(path.c_str(), &mWidth, &mHeight, 0, SOIL_LOAD_RGBA);
	}
}

}
