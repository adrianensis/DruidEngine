#ifndef DE_TEXTURE_H_
#define DE_TEXTURE_H_

#include "DE_Class.hpp"
#include <string>

namespace DE {

class Texture: public DE_Class {

	u32 mTextureId;
	int mWidth;
	int mHeight;
	byte* mData;
	std::string mPath;

public:

	DE_CLASS(Texture, DE_Class)

	void init(const std::string &path);

	DE_GET(TextureId)
	DE_GET(Data)
	DE_GET(Width)
	DE_GET(Height)
	DE_GET(Path)
};

} /* namespace DE */

#endif /* DE_TEXTURE_H_ */
