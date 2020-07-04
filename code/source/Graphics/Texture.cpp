#include "Texture.hpp"
#include "Log.hpp"
#include "SOIL.h"

namespace DE {

// ---------------------------------------------------------------------------

Texture::Texture() :
		DE_Class() {
	mTextureId = -1;
	mData = nullptr;
}

Texture::~Texture() {
	SOIL_free_image_data(mData);
}

// ---------------------------------------------------------------------------

void Texture::init(const std::string &path) {
	TRACE();

	if (!mData) {
		mPath = path;
		mData = SOIL_load_image(path.c_str(), &mWidth, &mHeight, 0,
				SOIL_LOAD_RGBA);
	}
}

// ---------------------------------------------------------------------------

} /* namespace DE */
