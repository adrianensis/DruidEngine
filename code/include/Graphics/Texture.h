#ifndef DE_TEXTURE_H_
#define DE_TEXTURE_H_

#include "DE_Class.h"
#include <string>

namespace DE {

class Texture : public DE_Class{

	u32 mTextureId;
	int mWidth;
	int mHeight;
	byte* mData;

public:
	DE_CLASS(Texture, DE_Class);

	void init(std::string path);

	bool isLoaded() const;

	DE_GET_SET(u32, TextureId);
	DE_GET(byte*, Data);
	DE_GET(u32, Width);
	DE_GET(u32, Height);
};

} /* namespace DE */

#endif /* DE_TEXTURE_H_ */
