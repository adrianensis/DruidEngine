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

	u32 getTextureId() const { return mTextureId;};
	void setTextureId(u32 newTextureId ) { mTextureId = newTextureId; };
	byte* getData() const { return mData; };
	u32 getWidth() const { return mWidth; };
	u32 getHeight() const { return mHeight; };
};

} /* namespace DE */

#endif /* DE_TEXTURE_H_ */
