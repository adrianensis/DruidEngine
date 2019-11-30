#include "Texture.h"
#include "Log.h"
#include "SOIL.h"

namespace DE {

// ---------------------------------------------------------------------------

Texture::Texture() : DE_Class() {
  mTextureId = -1;
  mData = nullptr;
}

Texture::~Texture() {
  SOIL_free_image_data(mData);
}

// ---------------------------------------------------------------------------

void Texture::init(std::string path){
	TRACE();

  // data = SOIL_load_image("resources/char.png", &mWidth, &mHeight, 0, SOIL_LOAD_RGB);
  mData = SOIL_load_image(path.c_str(), &mWidth, &mHeight, 0, SOIL_LOAD_RGBA);
}

// ---------------------------------------------------------------------------

bool Texture::isLoaded() const { return mData != nullptr;}

// ---------------------------------------------------------------------------

} /* namespace DE */
