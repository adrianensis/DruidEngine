#ifndef DE_TEXTURE_H_
#define DE_TEXTURE_H_

#include "DE_Class.hpp"
#include <string>

namespace DE {

class Texture : public DE_Class{

	u32 mTextureId;
	int mWidth;
	int mHeight;
	byte* mData;
	std::string mPath;

public:
	DE_CLASS(Texture, DE_Class);

	void init(const std::string& path);

	u32 getTextureId() const { return mTextureId; };
	const byte* getData() const { return mData; };
	u32 getWidth() const { return mWidth; };
	u32 getHeight() const { return mHeight; };
	const std::string& getPath() const { return mPath; };
};

} /* namespace DE */

#endif /* DE_TEXTURE_H_ */
