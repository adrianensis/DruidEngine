#include "Texture.h"

#include "SOIL.h"

namespace DE {

// ---------------------------------------------------------------------------

Texture::Texture() : DE_Class() {
  mTextureId = -1;
  data = nullptr;
}

Texture::~Texture() {
  SOIL_free_image_data(data);
}

// ---------------------------------------------------------------------------

void Texture::init(std::string path){
  // data = SOIL_load_image("resources/char.png", &mWidth, &mHeight, 0, SOIL_LOAD_RGB);
  data = SOIL_load_image(path.c_str(), &mWidth, &mHeight, 0, SOIL_LOAD_RGBA);
}

// ---------------------------------------------------------------------------

u32 Texture::getTextureId(){ return mTextureId; }
u32 Texture::setTextureId(u32 id){ mTextureId = id; }
bool Texture::isLoaded(){ return data != nullptr;}
byte* Texture::getData(){ return data; }
u32 Texture::getWidth(){ return mWidth; }
u32 Texture::getHeight(){ return mHeight; }

// ---------------------------------------------------------------------------

} /* namespace DE */
