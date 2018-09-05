#ifndef DE_BATCH_H
#define DE_BATCH_H

#include "DE_Class.h"
#include "Material.h"
#include "List.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Shader.h"

namespace DE {

class Batch : public DE_Class{
private:
	Material* mMaterial;
	List<Renderer*>* mRenderers;
	Mesh* mMesh;

	u32 mVBOPosition; // TODO: change u32 for GLuint
	u32 mEBO;
	u32 mVBOTexture;
	u32 mVBOColor;
	u32 mVBONormal;
	u32 mVAO;

	Shader* mShader;

public:
	Batch();
	~Batch() override;

	void init(Mesh* mesh, Material* material);
	void bind();
	void update();
	void render();
};

} /* namespace DE */

#endif /* DE_BATCH_H */
