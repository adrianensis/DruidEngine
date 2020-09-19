#include <TimeUtils.hpp>
#include "Batch.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "List.hpp"
#include "Renderer.hpp"
#include "RenderEngine.hpp"
#include "Camera.hpp"
#include "Frustum.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "RenderContext.hpp"
#include "List.hpp"
#include "HashMap.hpp"
#include "Log.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"
#include "MathUtils.hpp"
#include "Animation.hpp"
#include "Chunk.hpp"
#include "Settings.hpp"

namespace DE {

Matrix4 Batch::smScreenOrtho;
bool Batch::smIsScreenOrthoReady = false;

// ---------------------------------------------------------------------------

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



// ---------------------------------------------------------------------------

Batch::Batch() : DE_Class() {
	mVBOPosition = 0;
	mEBO = 0;
	mVBOTexture = 0;
	//mVBOColor = 0;
	mVBONormal = 0;
	mVAO = 0;
	mMesh = nullptr;
	mRenderers = nullptr;
	mMaterial = nullptr;
	mRenderEngine = nullptr;
	mTextureId = 0;

	mBinded = false;
}

Batch::~Batch() {
	FOR_LIST(itList, mRenderers->getValues()) {
		if (itList.get()) {
			VAR(u32, itList.get()->getLength());
			u32 remainingInBatch = itList.get()->getLength();
			FOR_LIST(itRenderer, itList.get()) {
				if (!itRenderer.get()->isDestroyed()) {
					itRenderer.get()->setDestroyed();
					Memory::free<Renderer>(itRenderer.get());
					remainingInBatch--;
				}
			}

			VAR(u32, remainingInBatch);

			Memory::free<List<Renderer*>>(itList.get());
		}
	}

	Memory::free<HashMap<u32, List<Renderer*>*>>(mRenderers);

	Memory::free<Array<f32>>(mPositionBuffer);
	Memory::free<Array<f32>>(mTextureBuffer);
	Memory::free<Array<u32>>(mFacesBuffer);

	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBOPosition);
	glDeleteBuffers(1, &mEBO);
}

// ---------------------------------------------------------------------------

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

	mPositionBuffer = Memory::allocate<Array<f32>>();
	mPositionBuffer->init(mMaxVertexBufferSize * mVertexPositionSize);
	mPositionBufferIndex = 0;

	mTextureBuffer = Memory::allocate<Array<f32>>();
	mTextureBuffer->init(mMaxVertexBufferSize * mVertexTextureSize);
	mTextureBufferIndex = 0;

	mColorBuffer = Memory::allocate<Array<f32>>();
	mColorBuffer->init(mMaxVertexBufferSize * mVertexColorSize);
	mColorBufferIndex = 0;

	mFacesBuffer = Memory::allocate<Array<u32>>();
	mFacesBuffer->init(mMaxVertexBufferSize * mFacesSize);

	mRenderEngine = RenderEngine::getInstance();

	mRenderers = Memory::allocate<HashMap<u32, List<Renderer*>*>>();
	mRenderers->init();

	FOR_RANGE(i, 0, mRenderEngine->getMaxLayers()) {
		mRenderers->set(i, nullptr);
	}

	mMesh = mesh;
	mMaterial = material;

	if (!smIsScreenOrthoReady) {
		smIsScreenOrthoReady = true;
		smScreenOrtho.ortho(-1 * RenderContext::getAspectRatio(), 1 * RenderContext::getAspectRatio(), -1, 1, 1, -1);
	}

	bind();
}

// ---------------------------------------------------------------------------

void Batch::bind() {
	mVAO = RenderContext::createVAO();
	mVBOPosition = RenderContext::createVBO(mVertexPositionSize, 0);
	mVBOTexture = RenderContext::createVBO(mVertexTextureSize, 1);
	mVBOColor = RenderContext::createVBO(mVertexColorSize, 2);
	//mVBONormal = RenderContext::createVBO(mMesh->getNormals(), 3, 3);
	mEBO = RenderContext::createEBO();

	FOR_RANGE(i, 0, mMaxMeshes) {
		FOR_RANGE(j, 0, 6) {
			mFacesBuffer->set(j + 6*i, mMesh->getFaces()->get(j) + (4*i));
		}
	}

	RenderContext::setDataEBO(mEBO, mFacesBuffer);

	Texture* texture = mMaterial->getTexture();

	if(texture) {
		glGenTextures(1, &mTextureId);

		glBindTexture(GL_TEXTURE_2D, mTextureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Prevents s-coordinate wrapping (repeating).
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		// Prevents t-coordinate wrapping (repeating).
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->getWidth(), texture->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
				texture->getData());
	}

	//glGenerateMipmap(GL_TEXTURE_2D);

	RenderContext::enableVAO(0);
}

// ---------------------------------------------------------------------------

void Batch::update() {

}

// ---------------------------------------------------------------------------

bool Batch::checkInFrustum(Camera *cam, Renderer *renderer) {
	Transform* t = renderer->getGameObject()->getTransform();

	Vector3 scale = t->getScale();
	f32 maxRadius = std::max(scale.x, scale.y); // TODO: if 3D, compare also with z

	Vector3 position(Vector3(t->getWorldPosition()).add(renderer->getPositionOffset()));

	return cam->getFrustum()->testSphere(position, maxRadius);
}

// ---------------------------------------------------------------------------

bool Batch::checkDistance(Camera *cam, Renderer *renderer) {
	Transform* t = renderer->getGameObject()->getTransform();

	Vector3 camPosition(cam->getGameObject()->getTransform()->getLocalPosition());
	Vector3 rendererPosition(t->getLocalPosition());

	return rendererPosition.dst(camPosition) < renderer->getRenderDistance();
}

// ---------------------------------------------------------------------------

bool Batch::checkOutOfCamera(Camera *cam, Renderer *renderer) {

	bool isOutOfCamera = false;

	if (renderer->isAffectedByProjection()) {
		renderer->setIsOutOfCamera(!checkInFrustum(cam, renderer));
		isOutOfCamera = renderer->isOutOfCamera();
	} else {
		isOutOfCamera = false;
	}

	return isOutOfCamera;
}

// ---------------------------------------------------------------------------

u32 Batch::render(u32 layer) {

	clearVertexBuffer();

	u32 drawCallCounter = 0;

	List<Renderer*>* renderers = mRenderers->get(layer);

	if (renderers && renderers->getLength() > 0) {

		bool isSortedLayer = mRenderEngine->getLayersData()->get(layer)->mSorted;

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
		shader->addBool(mMaterial->hasBorder(), "hasBorder");

		shader->addFloat(Time::getInstance()->getDeltaTimeSeconds(), "time");

		FOR_LIST(it, renderers) {
			Renderer* renderer = it.get();

			if(renderer->getLayer() == layer) {
				if(renderer->isActive()) {
					Chunk* chunk = renderer->getChunk();
					bool chunkOk = (!chunk) || (chunk && chunk->isLoaded());

					if (chunkOk) {
						Transform* t = renderer->getGameObject()->getTransform();

						if (!checkOutOfCamera(camera, renderer)) {

							shader->addBool(renderer->isAffectedByProjection(), "isAffectedByProjection");

							renderer->updateAnimation(mMaterial);

							bool lineMode = it.get()->isLineMode();

							addToVertexBuffer(renderer);

							drawCallCounter++;

							if (mRenderEngine->getDebugColliders()) {
								renderer->renderCollider();
							}

						}
					} else if (renderer->isAffectedByProjection() && !chunk->isLoaded()) {
						internalRemoveRendererFromList(&it, renderers);
					}

					if (isSortedLayer && renderer->isAffectedByProjection() && !renderer->isStatic()) {
						internalRemoveRendererFromList(&it, renderers);
					}

				} else if (renderer->isPendingToBeDestroyed()) {
					// destroy renderer and remove from list
					internalRemoveRendererFromList(&it, renderers);
				}
			}
		}

		if(mMeshesIndex > 0) {
			RenderContext::setDataVBO(mVBOPosition, mPositionBuffer);
			RenderContext::setDataVBO(mVBOTexture, mTextureBuffer);
			RenderContext::setDataVBO(mVBOColor, mColorBuffer);

			RenderContext::drawTriangles(mMeshesIndex * 6);
		}

		RenderContext::enableVAO(0);


	}

	return drawCallCounter;
}

// ---------------------------------------------------------------------------

void Batch::insertSorted(Renderer *renderer, List<Renderer*> *renderers) {

	// INSERT SORTED

	f32 y = renderer->getGameObject()->getTransform()->getWorldPosition().y;

	// CASE 1 : IF LIST IS EMPTY
	if (renderers->isEmpty()) {
		renderers->pushBack(renderer);
	} else {

		Renderer* first = renderers->getFirst().get();
		Renderer* last = renderers->getLast().get();

		// CASE 2 : RENDERER IS IN THE LAST/FIRST LAYER
		if (last->isActive() && (y <= last->getGameObject()->getTransform()->getWorldPosition().y)) {
			renderers->pushBack(renderer);
		} else if (first->isActive() && (y >= first->getGameObject()->getTransform()->getWorldPosition().y)) {
			renderers->pushFront(renderer);
		} else {

			// CASE 3 : LIST HAS ELEMENTS AND RENDERER IS IN A RANDOM LAYER, NOT THE LAST
			bool foundSmallerY = false;

			auto itSmallerY = renderers->getIterator();

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
			}
		}
	}
}

// ---------------------------------------------------------------------------

void Batch::addRenderer(Renderer *renderer) {

	checkOutOfCamera(mRenderEngine->getCamera(), renderer);

	u32 layer = renderer->getLayer();

	List<Renderer*>* renderers = mRenderers->get(layer);

	if (!renderers) {
		renderers = Memory::allocate<List<Renderer*>>();
		renderers->init();

		mRenderers->set(layer, renderers);
	}

	renderer->setIsAlreadyInBatch(true);

	if (!renderer->isStatic() && renderer->isAffectedByProjection()) {
		mRenderEngine->getLayersData()->get(renderer->getLayer())->mDynamicObjectsCount++;
	}

	if(mRenderEngine->getLayersData()->get(renderer->getLayer())->mSorted){
		insertSorted(renderer, renderers);
	} else {
		renderers->pushBack(renderer);
	}
}

// ---------------------------------------------------------------------------

void Batch::internalRemoveRenderer(const Iterator *it, List<Renderer*> *list) {
	internalRemoveRendererFromList(it, list);

	auto castedIt = it->cast<Renderer*>();
	Renderer* renderer = (*castedIt).get();


}
// ---------------------------------------------------------------------------

void Batch::internalRemoveRendererFromList(const Iterator *it, List<Renderer*> *list) {
	auto castedIt = it->cast<Renderer*>();
	list->remove(*castedIt);

	Renderer* renderer = (*castedIt).get();
	renderer->setIsAlreadyInBatch(false);

	if (!renderer->isStatic() && renderer->isAffectedByProjection()) {
		mRenderEngine->getLayersData()->get(renderer->getLayer())->mDynamicObjectsCount--;
	}

	// NOTE: UI CASE
	// UI is not Freed in Chunk so it has to ve freed here.
	if(! renderer->isAffectedByProjection()){
		renderer->setDestroyed();
		Memory::free<Renderer>(renderer);
	}
}

// ---------------------------------------------------------------------------

void Batch::addToVertexBuffer(Renderer* renderer) {

	Transform* t = renderer->getGameObject()->getTransform();


	Array<Vector2>* vertexPositions = renderer->getVertices();

	FOR_RANGE(i,0,mVerticesPerMesh) {

		Vector3 vertexPosition(vertexPositions->get(i));

		mPositionBuffer->set(mPositionBufferIndex, vertexPosition.x); mPositionBufferIndex++;
		mPositionBuffer->set(mPositionBufferIndex, vertexPosition.y); mPositionBufferIndex++;
		mPositionBuffer->set(mPositionBufferIndex, vertexPosition.z); mPositionBufferIndex++;

		Vector2 vertexTexture(
		mMesh->getTextureCoordinates()->get(i*mVertexTextureSize + 0),
		mMesh->getTextureCoordinates()->get(i*mVertexTextureSize + 1));

		Vector2 regionSize = renderer->getRegionSize();
		Vector2 regionPosition = renderer->getRegionPosition();

		Vector2 textureCoord(vertexTexture.x*regionSize.x + regionPosition.x, (1.0f-vertexTexture.y)*regionSize.y + regionPosition.y);

		if(renderer->getInvertXAxis()){

			textureCoord.x = 1.0f - textureCoord.x;

			const Animation* animation = renderer->getCurrentAnimation();

			if(animation) {
				textureCoord.x = textureCoord.x - (1.0f - (animation->getNumberOfFrames()*regionSize.x));
			}
		}

		mTextureBuffer->set(mTextureBufferIndex, textureCoord.x); mTextureBufferIndex++;
		mTextureBuffer->set(mTextureBufferIndex, textureCoord.y); mTextureBufferIndex++;

		mColorBuffer->set(mColorBufferIndex, renderer->getColor()->get(0)); mColorBufferIndex++;
		mColorBuffer->set(mColorBufferIndex, renderer->getColor()->get(1)); mColorBufferIndex++;
		mColorBuffer->set(mColorBufferIndex, renderer->getColor()->get(2)); mColorBufferIndex++;
		mColorBuffer->set(mColorBufferIndex, renderer->getColor()->get(3)); mColorBufferIndex++;
	}

	mMeshesIndex++;
}

void Batch::clearVertexBuffer() {
	mPositionBufferIndex = 0;
	mTextureBufferIndex = 0;
	mColorBufferIndex = 0;

	mMeshesIndex = 0;
}

} /* namespace DE */
