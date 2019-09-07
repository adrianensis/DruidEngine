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

	u32 getTextureId() const;
	u32 setTextureId(u32 id);
	bool isLoaded() const;
	byte* getData() const;
	u32 getWidth() const;
	u32 getHeight() const;
};

} /* namespace DE */

#endif /* DE_TEXTURE_H_ */
