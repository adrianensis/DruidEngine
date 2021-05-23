#include "Core/Time/TimeUtils.hpp"
#include "Graphics/Batch.hpp"
#include "Graphics/Material.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/RenderEngine.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Frustum.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/RenderContext.hpp"
#include "Log/Log.hpp"
#include "Scene/GameObject.hpp"
#include "Scene/Transform.hpp"
#include "Maths/MathUtils.hpp"
#include "Graphics/Animation/Animation.hpp"
#include "Graphics/Chunk.hpp"

Matrix4 Batch::smScreenOrtho;
bool Batch::smIsScreenOrthoReady = false;

u8 Batch::rendererYCoordinateComparator(Renderer *a, Renderer *b) {

	/*
	 We want objects with:

	 BIGGER y coordinate -> BEHIND
	 SMALLER y coordinate -> FRONT
	 */

	f32 aY = a->getGameObject()->getTransform()->getWorldPosition().y;
	f32 bY = b->getGameObject()->getTransform()->getWorldPosition().y;

	return (aY > bY) ? 1 : (bY > aY) ? 2 : 0;
}

Batch::Batch() {
	mVBOPosition = 0;
	mEBO = 0;
	mVBOTexture = 0;
	mVBOColor = 0;
	mVBONormal = 0;
	mVAO = 0;
	mMesh = nullptr;
	mMaterial = nullptr;
	mRenderEngine = nullptr;
	mTextureId = 0;

	mBinded = false;
}

Batch::~Batch() {
	FOR_MAP(itList, mRenderers) {
		if (itList->second) {
			std::list<Renderer*>* rendereresList = itList->second;
			u32 remainingInBatch = rendereresList->size();
			FOR_LIST(itRenderer, *rendereresList) {
				if (!(*itRenderer)->getIsDestroyed()) {
					(*itRenderer)->finallyDestroy();
					delete (*itRenderer);
					remainingInBatch--;
				}
			}

			delete rendereresList;
		}
	}

	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBOPosition);
	glDeleteBuffers(1, &mEBO);
}

void Batch::init(const Mesh *mesh, Material *material) {
	// TRACE();

	mMaxMeshes = 2000;
	mVerticesPerMesh = 4;
	mMeshesIndex = 0;
	mMaxVertexBufferSize = mVerticesPerMesh * mMaxMeshes;
	mVertexPositionSize = 3;
	mVertexTextureSize = 2;
	mVertexColorSize = 4;
	mFacesSize = 6;

	mPositionBuffer.reserve(mMaxVertexBufferSize * mVertexPositionSize);
	mPositionBufferIndex = 0;

	mTextureBuffer.reserve(mMaxVertexBufferSize * mVertexTextureSize);
	mTextureBufferIndex = 0;

	mColorBuffer.reserve(mMaxVertexBufferSize * mVertexColorSize);
	mColorBufferIndex = 0;

	mIndicesBuffer.reserve(mMaxVertexBufferSize * mFacesSize);

	mRenderEngine = RenderEngine::getInstance();

	FOR_RANGE(i, 0, mRenderEngine->getMaxLayers()) {
		MAP_INSERT(mRenderers, i, nullptr)
	}

	mMesh = mesh;
	mMaterial = material;

	if (!smIsScreenOrthoReady) {
		smIsScreenOrthoReady = true;
		smScreenOrtho.ortho(-1 * RenderContext::getAspectRatio(), 1 * RenderContext::getAspectRatio(), -1, 1, 1, -1);
	}

	bind();
}

void Batch::bind() {
	mVAO = RenderContext::createVAO();
	mVBOPosition = RenderContext::createVBO(mVertexPositionSize, 0);
	mVBOTexture = RenderContext::createVBO(mVertexTextureSize, 1);
	mVBOColor = RenderContext::createVBO(mVertexColorSize, 2);
	//mVBONormal = RenderContext::createVBO(mMesh->getNormals(), 3, 3);
	mEBO = RenderContext::createEBO();

	FOR_RANGE(i, 0, mMaxMeshes) {
		FOR_RANGE(j, 0, 6) {
			mIndicesBuffer[j + 6*i] = mMesh->getFaces()[j] + (4*i);
		}
	}

	RenderContext::setDataEBO(mEBO, mIndicesBuffer);

	Texture* texture = mMaterial->getTexture();

	if(texture) {
		glGenTextures(1, &mTextureId);

		glBindTexture(GL_TEXTURE_2D, mTextureId);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->getWidth(), texture->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
						texture->getData());

		//glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_TEXTURE_MAG_FILTER
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	}

	RenderContext::enableVAO(0);
}

void Batch::update() {

}

bool Batch::checkInFrustum(Camera *cam, Renderer *renderer) {
	Transform* t = renderer->getGameObject()->getTransform();

	Vector3 scale = t->getScale();
	f32 maxRadius = std::max(scale.x, scale.y); // TODO: if 3D, compare also with z

	Vector3 position(Vector3(t->getWorldPosition()).add(renderer->getPositionOffset()));

	return cam->getFrustum()->testSphere(position, maxRadius);
}

bool Batch::checkDistance(Camera *cam, Renderer *renderer) {
	Transform* t = renderer->getGameObject()->getTransform();

	Vector3 camPosition(cam->getGameObject()->getTransform()->getLocalPosition());
	Vector3 rendererPosition(t->getLocalPosition());

	return rendererPosition.dst(camPosition) < renderer->getRenderDistance();
}

bool Batch::checkIsOutOfCamera(Camera *cam, Renderer *renderer) {

	bool getIsOutOfCamera = false;

	if (renderer->getIsAffectedByProjection()) {
		renderer->setIsOutOfCamera(!checkInFrustum(cam, renderer));
		getIsOutOfCamera = renderer->getIsOutOfCamera();
	} else {
		getIsOutOfCamera = false;
	}

	return getIsOutOfCamera;
}

u32 Batch::render(u32 layer) {

	clearVertexBuffer();

	u32 drawCallCounter = 0;

	std::list<Renderer*>* renderers = mRenderers[layer];

	if (renderers && renderers->size() > 0) {

		bool isSortedLayer = mRenderEngine->getLayersData().at(layer)->mSorted;

		Shader* shader = mMaterial->getShader();

		shader->use();
		RenderContext::enableVAO(mVAO);

		if(mMaterial->getTexture()) {
			glBindTexture(GL_TEXTURE_2D, mTextureId);
		}

		Camera* camera = mRenderEngine->getCamera();

		const Matrix4& projectionMatrix = camera->getProjectionMatrix();
		const Matrix4& viewTranslationMatrix = camera->getViewTranslationMatrix();
		const Matrix4& viewRotationMatrix = camera->getViewRotationMatrix();

		shader->addMatrix(projectionMatrix, "projectionMatrix");
		shader->addMatrix(viewTranslationMatrix, "viewTranslationMatrix");
		shader->addMatrix(viewRotationMatrix, "viewRotationMatrix");

		shader->addBool(mMaterial->getTexture() != nullptr, "hasTexture");
		shader->addBool(mMaterial->getAlphaEnabled(), "alphaEnabled");
		shader->addBool(mMaterial->getHasBorder(), "hasBorder");

		shader->addFloat(Time::getInstance()->getDeltaTimeSeconds(), "time");

		FOR_LIST(it, *renderers) {
			Renderer* renderer = *it;

			if(renderer->getLayer() == layer) {

				bool toRemove = false;
				if(renderer->isActive()) {
					const Chunk* chunk = renderer->getChunk();
					bool chunkOk = (!chunk) || (chunk && chunk->getIsLoaded());

					if (chunkOk) {
						Transform* t = renderer->getGameObject()->getTransform();

						if (!checkIsOutOfCamera(camera, renderer)) {

							shader->addBool(renderer->getIsAffectedByProjection(), "isAffectedByProjection");

							bool IsLineMode = renderer->getIsLineMode();

							renderer->updateAnimation();
							addToVertexBuffer(renderer);

							drawCallCounter++;
						}
					} else if (renderer->getIsAffectedByProjection() && !chunk->getIsLoaded()) {
						toRemove = true;
					}

					if (isSortedLayer && renderer->getIsAffectedByProjection() && !renderer->isStatic()) {
						toRemove = true;
					}

				} else if (renderer->getIsPendingToBeDestroyed()) {
					// destroy renderer and remove from list
					toRemove = true;
				}

				if(toRemove){
					internalRemoveRendererFromList(it, renderers);
				}
			}
		}

		if(mMeshesIndex > 0) {
			RenderContext::setDataVBO(mVBOPosition, mPositionBuffer);
			RenderContext::setDataVBO(mVBOTexture, mTextureBuffer);
			RenderContext::setDataVBO(mVBOColor, mColorBuffer);

			RenderContext::drawRectangles(mMeshesIndex);
		}

		RenderContext::enableVAO(0);

	}

	return drawCallCounter;
}

void Batch::insertSorted(Renderer *renderer, std::list<Renderer*> *renderers) {

	// INSERT SORTED

	f32 y = renderer->getGameObject()->getTransform()->getWorldPosition().y;

	// CASE 1 : IF LIST IS EMPTY
	if (renderers->empty()) {
		renderers->push_back(renderer);
	} else {

		Renderer* first = *renderers->begin();
		Renderer* last = *renderers->end();

		// CASE 2 : RENDERER IS IN THE LAST/FIRST LAYER
		if (last->isActive() && (y <= last->getGameObject()->getTransform()->getWorldPosition().y)) {
			renderers->push_back(renderer);
		} else if (first->isActive() && (y >= first->getGameObject()->getTransform()->getWorldPosition().y)) {
			renderers->push_front(renderer);
		} else {

			// CASE 3 : LIST HAS ELEMENTS AND RENDERER IS IN A RANDOM LAYER, NOT THE LAST
			bool foundSmallerY = false;

			// TODO : complete insert sorted
			/*auto itSmallerY = renderers->getIterator();

			FOR_LIST_COND(it, renderers, !foundSmallerY)
			{
				Renderer* otherRenderer = it.get();
				if(otherRenderer->isActive()){
					f32 otherY = otherRenderer->getGameObject()->getTransform()->getWorldPosition().y;

					if (y >= otherY) {
						foundSmallerY = true;
						itSmallerY = it;
					}
				}
			}

			if (foundSmallerY) {
				renderers->insert(itSmallerY, renderer); // this method inserts before the iterator
			} else {
				renderers->pushBack(renderer);
			}*/

			renderers->push_back(renderer);
		}
	}
}

void Batch::addRenderer(Renderer *renderer) {

	checkIsOutOfCamera(mRenderEngine->getCamera(), renderer);

	u32 layer = renderer->getLayer();

	std::list<Renderer*>* renderers = mRenderers[layer];

	if (!renderers) {
		renderers = new std::list<Renderer*>;

		MAP_INSERT(mRenderers, layer, renderers);
	}

	renderer->setIsAlreadyInBatch(true);

	if (!renderer->isStatic() && renderer->getIsAffectedByProjection()) {
		mRenderEngine->getLayersData().at(renderer->getLayer())->mDynamicObjectsCount++;
	}

	if(mRenderEngine->getLayersData().at(renderer->getLayer())->mSorted){
		insertSorted(renderer, renderers);
	} else {
		renderers->push_back(renderer);
	}
}

void Batch::internalRemoveRendererFromList(std::list<Renderer*>::iterator &it, std::list<Renderer*> *list) {
	list->erase(it);

	Renderer* renderer = *it;
	renderer->setIsAlreadyInBatch(false);

	if(renderer->getIsAffectedByProjection()) {
		if (!renderer->isStatic()) {
			mRenderEngine->getLayersData().at(renderer->getLayer())->mDynamicObjectsCount--;
		}
	} else {
		// NOTE: UI CASE
		// UI is not Freed in Chunk so it has to be freed here.
		renderer->finallyDestroy();
		delete renderer;
	}
}

void Batch::addToVertexBuffer(Renderer* renderer) {

	Transform* t = renderer->getGameObject()->getTransform();

	const std::vector<Vector2>* vertexPositions = renderer->getVertices();

	FOR_RANGE(i,0,mVerticesPerMesh) {

		Vector3 vertexPosition(vertexPositions->at(i));

		mPositionBuffer[mPositionBufferIndex] = vertexPosition.x; mPositionBufferIndex++;
		mPositionBuffer[mPositionBufferIndex] = vertexPosition.y; mPositionBufferIndex++;
		mPositionBuffer[mPositionBufferIndex] = vertexPosition.z; mPositionBufferIndex++;

		Vector2 vertexTexture(
		mMesh->getTextureCoordinates()[i*mVertexTextureSize + 0],
		mMesh->getTextureCoordinates()[i*mVertexTextureSize + 1]);

		Vector2 regionSize = renderer->getRegionSize();
		Vector2 regionPosition = renderer->getRegionPosition();

		Vector2 textureCoord(vertexTexture.x*regionSize.x + regionPosition.x, (1.0f-vertexTexture.y)*regionSize.y + regionPosition.y);

		if(renderer->getIsInvertAxis()){

			textureCoord.x = 1.0f - textureCoord.x;

			const Animation* animation = renderer->getCurrentAnimation();

			if(animation) {
				textureCoord.x = textureCoord.x - (1.0f - (animation->getNumberOfFrames()*regionSize.x));
			}
		}

		mTextureBuffer[mTextureBufferIndex] = textureCoord.x; mTextureBufferIndex++;
		mTextureBuffer[mTextureBufferIndex] = textureCoord.y; mTextureBufferIndex++;

		mColorBuffer[mColorBufferIndex] = renderer->getColor()[0]; mColorBufferIndex++;
		mColorBuffer[mColorBufferIndex] = renderer->getColor()[1]; mColorBufferIndex++;
		mColorBuffer[mColorBufferIndex] = renderer->getColor()[2]; mColorBufferIndex++;
		mColorBuffer[mColorBufferIndex] = renderer->getColor()[3]; mColorBufferIndex++;
	}

	mMeshesIndex++;
}

void Batch::clearVertexBuffer() {
	mPositionBufferIndex = 0;
	mTextureBufferIndex = 0;
	mColorBufferIndex = 0;

	mMeshesIndex = 0;
}