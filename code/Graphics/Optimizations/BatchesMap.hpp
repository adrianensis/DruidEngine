#pragma once

#include "Core/Core.hpp"
#include "Maths/Maths.hpp"

class Texture;
class Material;
class Renderer;
class Mesh;
class Shader;
class Batch;

class BatchesMap: public ObjectBase
{
    GENERATE_METADATA(BatchesMap)

	// Simplifies the batch mapping by texture, mesh and shader
	PRI class BatchKey
	{
		PUB Texture* mTexture = nullptr;
		PUB Shader* mShader = nullptr;
		PUB Mesh* mMesh = nullptr;

		PUB void init(Texture* texture, Shader* shader, Mesh* mesh)
		{
			mTexture = texture;
			mShader = shader;
			mMesh = mesh;
		}

		PUB bool operator==(const BatchKey &otherBatchKey) const
		{
			return mTexture == otherBatchKey.mTexture && mShader == otherBatchKey.mShader && mMesh == otherBatchKey.mMesh;
		}
	};

	PRI std::vector<BatchKey> mBatchKeys;

	PRI std::map<BatchKey*, Batch*> mBatchesDynamic;
	PRI std::map<BatchKey*, Batch*> mBatchesDynamicScreenSpace;
	PRI std::map<BatchKey*, Batch*> mBatchesStatic;
	PRI std::map<BatchKey*, Batch*> mBatchesStaticScreenSpace;

	void renderBatchesMap(std::map<BatchKey*, Batch*>& batchesMap);

PUB
	~BatchesMap() override;

	void init();
	void render();
	void addRenderer(Renderer * renderer);

	void forceRegenerateBuffers();
};