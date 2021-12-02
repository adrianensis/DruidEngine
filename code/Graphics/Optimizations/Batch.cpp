#include "Graphics/Optimizations/Batch.hpp"
#include "Graphics/Material/Material.hpp"
#include "Graphics/Material/Texture.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/Camera/Camera.hpp"
#include "Graphics/Camera/Frustum.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Material/Shader.hpp"
#include "Graphics/RenderContext.hpp"
#include "Graphics/Animation/Animation.hpp"
#include "Graphics/Optimizations/Chunk.hpp"
#include "Scenes/Scenes.hpp"

Batch::~Batch()
{
	FOR_LIST(it, mProxyRenderers)
	{
		if (it->isValid())
		{
			it->disconnectFromOwner();
			DELETE(it->getObject());
		}
	}

	mProxyRenderers.clear();

	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBOPosition);
	glDeleteBuffers(1, &mVBOTexture);
	glDeleteBuffers(1, &mVBOColor);
	//glDeleteBuffers(1, &mVBONormal);
	glDeleteBuffers(1, &mVBOMatrices);
	glDeleteBuffers(1, &mEBO);
}

void Batch::init(const Mesh *mesh, Material *material)
{
	// TRACE();

	mMesh = mesh;
	mMaterial = material;
	mIsWorldSpace = true;

	mMeshesIndex = 0;

	bind();
}

void Batch::bind()
{
	mVAO = RenderContext::createVAO();
	mVBOPosition = RenderContext::createVBO(Mesh::smVertexPositionSize, 0);
	mVBOTexture = RenderContext::createVBO(Mesh::smVertexTexCoordSize, 1);
	mVBOColor = RenderContext::createVBO(Mesh::smVertexColorSize, 2);
	//mVBONormal = RenderContext::createVBO(mMesh->getNormals(), 3, 3);

	if(getIsInstanced())
	{
		glGenBuffers(1, &mVBOMatrices);
		glBindBuffer(GL_ARRAY_BUFFER, mVBOMatrices);

		// vertex attributes
		u32 columnBytesSize = Matrix4::smColumnSize * sizeof(f32);
		RenderContext::enableProperty(3);
		glVertexAttribPointer(3, Matrix4::smColumnSize, GL_FLOAT, GL_FALSE, 4 * columnBytesSize, (void*)0);
		RenderContext::enableProperty(4);
		glVertexAttribPointer(4, Matrix4::smColumnSize, GL_FLOAT, GL_FALSE, 4 * columnBytesSize, (void*)(1 * columnBytesSize));
		RenderContext::enableProperty(5);
		glVertexAttribPointer(5, Matrix4::smColumnSize, GL_FLOAT, GL_FALSE, 4 * columnBytesSize, (void*)(2 * columnBytesSize));
		RenderContext::enableProperty(6);
		glVertexAttribPointer(6, Matrix4::smColumnSize, GL_FLOAT, GL_FALSE, 4 * columnBytesSize, (void*)(3 * columnBytesSize));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
	}

	mEBO = RenderContext::createEBO();
	
	Texture *texture = mMaterial->getTexture();

	if (texture)
	{
		texture->bind();
	}
	
	RenderContext::enableVAO(0);
}

void Batch::render()
{
	PROFILER_TIMEMARK_START()

	if (!mProxyRenderers.empty())
	{
		RenderContext::enableVAO(mVAO);

		mMaterial->enable();

		mMaterial->bind(getIsWorldSpace(), getIsInstanced());

		if(shouldRegenerateBuffers())
		{
			mPendingDrawCall = processRenderers();
		}
		else
		{
			mPendingDrawCall = true;
		}

		if(mPendingDrawCall)
		{
			drawCall(); // flush all the previous rendereres
		}

		mMaterial->disable();

		RenderContext::enableVAO(0);
	}

	PROFILER_TIMEMARK_END()
}

void Batch::resizeBuffers()
{
	PROFILER_TIMEMARK_START()

	mMeshBuilder.clear();

	if(getIsInstanced())
	{
		mMatrices.clear();
	}
	
	u32 newSize = mProxyRenderers.size();
	if (newSize > mMaxMeshesThreshold)
	{
		mMaxMeshesThreshold += mMaxMeshesIncrement;

		u32 meshesAmount = getIsInstanced() ? 1 : mMaxMeshesThreshold; 

		mMeshBuilder.init(mMesh->getVertexCount() * meshesAmount, mMesh->getFacesCount() * meshesAmount);

		mMatrices.reserve(Matrix4::smMatrixSize * mMaxMeshesThreshold);

		generateFacesData(meshesAmount);
	}

	if(getIsInstanced())
	{
		mMeshBuilder.copyVertices(mMesh);
		mMeshBuilder.copyTextureCoordinates(mMesh);
		//mMeshBuilder.copyColors(mMesh);

		FOR_RANGE(i, 0, mMesh->getVertexCount())
		{
			mMeshBuilder.addColor(0,0,0,1);
		}
	}

	invalidateAndReallocateBuffers();
	
	PROFILER_TIMEMARK_END()
}

bool Batch::processRenderers()
{
	PROFILER_TIMEMARK_START()

	resizeBuffers();

	bool pendingDrawCall = false;
	
	FOR_LIST(it, mProxyRenderers)
	{
		bool toRemove = false;

		if(it->isValid())
		{
			Renderer *renderer = it->getObject();
			if (renderer->isActive())
			{
				if (isChunkOk(renderer))
				{
					/*Transform* transform = renderer->getGameObject()->getTransform();
					const Vector3& position = transform->getWorldPosition();
					f32 distanceToCamera = position.dst(RenderEngine::getInstance().getCamera()->getGameObject()->getTransform()->getWorldPosition());
					if(!renderer->getIsWorldSpace() || distanceToCamera <= renderer->getRenderDistance())*/
					
					if(renderer->hasClipRectangle())
					{
						/*if(pendingDrawCall)
						{
							drawCall(); // flush all the previous rendereres
							resizeBuffers(renderers->size()); // TODO : resize to the correct remaining size
						}

						addToVertexBuffer(renderer);

						mMaterial->getShader()->addVector2(renderer->getClipRectangle().getLeftTop(), "clipRegionLeftTop");
						mMaterial->getShader()->addVector2(renderer->getClipRectangle().getSize(), "clipRegionSize");

						drawCall();
						resizeBuffers(renderers->size());

						// TODO : comment this ↓↓↓↓ to test clip rectangle
						mMaterial->getShader()->addVector2(Vector2(), "clipRegionLeftTop");
						mMaterial->getShader()->addVector2(Vector2(), "clipRegionSize");*/
					}
					else
					{
						addToVertexBuffer(renderer);
						pendingDrawCall = true;
					}
				}
				else
				{
					toRemove = true;
				}
			}
			else
			{
				if (renderer->getIsPendingToBeDestroyed())
				{
					toRemove = true;
				}
			}
		}
		else
		{
			toRemove = true;
		}

		if (toRemove)
		{
			internalRemoveRendererFromList(it);
		}
	}

	PROFILER_TIMEMARK_END()

	return pendingDrawCall;
}

bool Batch::isChunkOk(Renderer *renderer) const
{
	const Chunk *chunk = renderer->getChunk();
	return (!chunk) || (chunk && chunk->getIsLoaded()); // !chunk means -> Screen Space case
}

void Batch::drawCall()
{
	PROFILER_TIMEMARK_START()
	if (mMeshesIndex > 0)
	{
		sendDataToBuffers();

		RenderContext::drawElements(mMesh->getFaces().size(), mMeshesIndex, getIsInstanced());
	}

	mNewRendererAdded = false;
	mPendingDrawCall = false;
	mForceRegenerateBuffers = false;

	PROFILER_TIMEMARK_END()
}

void Batch::addRenderer(Renderer *renderer)
{
	ProxyObject<Renderer> proxy;

	mProxyRenderers.push_back(proxy);

	mProxyRenderers.back().init(renderer);

	renderer->setBatch(this);

	mNewRendererAdded = true;
}

void Batch::internalRemoveRendererFromList(std::list<ProxyObject<Renderer>>::iterator &it)
{
	PROFILER_TIMEMARK_START()

	ProxyObject<Renderer>& proxy = *it;

	proxy.disconnectFromOwner();

	if(proxy.isValid())
	{
		Renderer* renderer = proxy.getObject();
		renderer->setBatch(nullptr);

		// NOTE: UI CASE
		// UI is not deleted in Chunk so it has to be deleted here.
		if (!getIsWorldSpace())
		{
			renderer->finallyDestroy();
			DELETE(renderer);
		}
	}

	it = mProxyRenderers.erase(it);
	--it; // go back to the previous it, so the FOR LOOP can do ++it with no problem

	PROFILER_TIMEMARK_END()
}

void Batch::addToVertexBuffer(Renderer *renderer)
{
	renderer->updateAnimation();

	if(getIsInstanced())
	{
		addToVertexBufferInstanced(renderer);
	}
	else
	{
		addToVertexBufferNotInstanced(renderer);
	}

	mMeshesIndex++;
}

void Batch::addToVertexBufferNotInstanced(Renderer * renderer)
{
	PROFILER_TIMEMARK_START()

	const std::vector<Vector3> &vertexPositions = renderer->getVertices();

	FOR_RANGE(i, 0, mMesh->getVertexCount())
	{
		mMeshBuilder.addVertex(vertexPositions[i]);

		Vector2 vertexTexture(
			mMesh->getTextureCoordinates()[i * Mesh::smVertexTexCoordSize + 0],
			mMesh->getTextureCoordinates()[i * Mesh::smVertexTexCoordSize + 1]);

		Vector2 regionSize = renderer->getRegion().getSize();
		Vector2 regionPosition = renderer->getRegion().getLeftTop();

		Vector2 textureCoord(vertexTexture.x * regionSize.x + regionPosition.x, vertexTexture.y * regionSize.y + regionPosition.y);

		if (renderer->getInvertAxisX())
		{
			textureCoord.x = 1.0f - textureCoord.x;

			const Animation *animation = renderer->getCurrentAnimation();

			if (animation)
			{
				textureCoord.x = textureCoord.x - (1.0f - (animation->getNumberOfFrames() * regionSize.x));
			}
		}

		mMeshBuilder.addTexCoord(textureCoord.x, textureCoord.y);

		mMeshBuilder.addColor(
			renderer->getColor()[0],
			renderer->getColor()[1],
			renderer->getColor()[2],
			renderer->getColor()[3]);
	}

	PROFILER_TIMEMARK_END()
}

void Batch::addToVertexBufferInstanced(Renderer * renderer)
{
	PROFILER_TIMEMARK_START()

	const Matrix4& rendererModelMatrix = renderer->getRendererModelMatrix();
	std::copy(rendererModelMatrix.getData(), rendererModelMatrix.getData() + Matrix4::smMatrixSize, back_inserter(mMatrices));

	PROFILER_TIMEMARK_END()
}

void Batch::invalidateAndReallocateBuffers()
{
	// Invalidate buffers so OpenGL will allocate new one under the hood, reducing sync
	RenderContext::resizeVBO(mVBOPosition, mMeshBuilder.getVertices().capacity(), mIsStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	RenderContext::resizeVBO(mVBOTexture, mMeshBuilder.getTextureCoordinates().capacity(), mIsStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	RenderContext::resizeVBO(mVBOColor, mMeshBuilder.getColors().capacity(), mIsStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

	if(getIsInstanced())
	{
		RenderContext::resizeVBO(mVBOMatrices, mMatrices.capacity(), mIsStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	}

	mMeshesIndex = 0;
	mDataSentToGPU = false;
}

void Batch::generateFacesData(u32 meshesCount)
{
	// Create Faces once and send to GPU once.
	FOR_RANGE(i, 0, meshesCount)
	{
		u32 offset = (i * mMesh->getVertexCount());
		
		FOR_RANGE(faceIndex, 0, mMesh->getFaces().size())
		{
			mMeshBuilder.addFaceIndex(mMesh->getFaces()[faceIndex] + offset);
		}
	}

	RenderContext::resizeEBO(mEBO, mMeshBuilder.getFaces().size(), mIsStatic ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
	RenderContext::setDataEBO(mEBO, mMeshBuilder.getFaces());
}

void Batch::sendDataToBuffers()
{
	if(!mDataSentToGPU)
	{
		RenderContext::setDataVBO(mVBOPosition, mMeshBuilder.getVertices());
		RenderContext::setDataVBO(mVBOTexture, mMeshBuilder.getTextureCoordinates());
		RenderContext::setDataVBO(mVBOColor, mMeshBuilder.getColors());

		if(getIsInstanced())
		{
			RenderContext::setDataVBO(mVBOMatrices, mMatrices);
		}

		mDataSentToGPU = true;
	}
}

bool Batch::shouldRegenerateBuffers() const
{
	return mNewRendererAdded || !mIsStatic || mForceRegenerateBuffers;
}
