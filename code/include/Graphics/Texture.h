#ifndef DE_TEXTURE_H_
#define DE_TEXTURE_H_

#include "DE_Class.h"
#include <string>

namespace DE {

class Texture : public DE_Class{

	u32 mTextureId;
	int mWidth;
	int mHeight;
	byte* data;

public:
	DE_CLASS(Texture, DE_Class);

	void init(std::string path);

	u32 getTextureId();
	u32 setTextureId(u32 id);
	bool isLoaded();
	byte* getData();
	u32 getWidth();
	u32 getHeight();
};

} /* namespace DE */

#endif /* DE_TEXTURE_H_ */
