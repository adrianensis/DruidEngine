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
#include "Time2.hpp"
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

	mChunk = nullptr;
}

Batch::~Batch() {

	FOR_LIST(itList, mRenderers->getValues()) {
		if (itList.get()) {
			FOR_LIST(itRenderer, itList.get()) {
				if (!itRenderer.get()->isDestroyed()) {
					itRenderer.get()->setDestroyed();
					Memory::free<Renderer>(itRenderer.get());
				}
			}

			Memory::free<List<Renderer*>>(itList.get());
		}
	}

	Memory::free<HashMap<u32, List<Renderer*>*>>(mRenderers);



	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBOPosition);
	glDeleteBuffers(1, &mEBO);
}

// ---------------------------------------------------------------------------

void Batch::init(const Mesh *mesh, Material *material) {
	// TRACE();

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
	mVBOPosition = RenderContext::createVBO(mMesh->getVertices(), 3, 0);
	mVBOTexture = RenderContext::createVBO(mMesh->getTextureCoordinates(), 2, 1);
	//mVBOColor = RenderContext::createVBO(mMesh->getColors(), 4, 2);
	//mVBONormal = RenderContext::createVBO(mMesh->getNormals(), 3, 3);
	mEBO = RenderContext::createEBO(mMesh->getFaces());

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

		FOR_LIST(it, renderers)
		{
			Renderer* renderer = it.get();

			Chunk* chunk = renderer->getChunk();
			bool chunkOk = (!chunk) || (chunk && chunk->isLoaded());

			if (renderer->isActive() && chunkOk) {

				Transform* t = renderer->getGameObject()->getTransform();

				if (renderer->getLayer() == layer && !checkOutOfCamera(camera, renderer)) {

					const Matrix4& translationMatrix = t->getTranslationMatrix();
					const Matrix4& rotationMatrix = t->getRotationMatrix();
					const Matrix4& scaleMatrix = t->getScaleMatrix();

					shader->addMatrix(translationMatrix, "translationMatrix");
					shader->addMatrix(renderer->getPositionOffsetMatrix(), "positionOffsetMatrix");
					shader->addMatrix(rotationMatrix, "rotationMatrix");
					shader->addMatrix(scaleMatrix, "scaleMatrix");

					if (renderer->isAffectedByProjection()) {
						shader->addMatrix(projectionMatrix, "projectionMatrix");
						shader->addMatrix(viewTranslationMatrix, "viewTranslationMatrix");
						shader->addMatrix(viewRotationMatrix, "viewRotationMatrix");
					} else {
						shader->addMatrix(Matrix4::getIdentity(), "projectionMatrix");
						shader->addMatrix(Matrix4::getIdentity(), "viewTranslationMatrix");
						shader->addMatrix(Matrix4::getIdentity(), "viewRotationMatrix");
					}

					shader->addFloat(Time::getDeltaTimeSeconds(), "time");

					renderer->updateMaterial(mMaterial);

					bool lineMode = it.get()->isLineMode();

					glPolygonMode(GL_FRONT_AND_BACK, lineMode ? GL_LINE : GL_FILL);

					glDrawElements(GL_TRIANGLES, mMesh->getFaces()->getLength(),
					GL_UNSIGNED_INT, 0);

					drawCallCounter++;

					if (mRenderEngine->getDebugColliders()) {
						renderer->renderCollider();
					}

				} else if (renderer->isPendingToBeDestroyed()) {
					// destroy renderer and remove from list
					internalRemoveRenderer(&it, renderers);
				}
			} else if (renderer->isAffectedByProjection()) {
				if (!chunk->isLoaded()) {
					internalRemoveRendererFromList(&it, renderers);
				}
			}

			if (isSortedLayer && renderer->isAffectedByProjection() && !renderer->isStatic()) {
				internalRemoveRendererFromList(&it, renderers);
			}

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

		// CASE 2 : RENDERER IS IN THE LAST/FIRST LAYER
		if (y <= renderers->getLast().get()->getGameObject()->getTransform()->getWorldPosition().y) {
			renderers->pushBack(renderer);
		} else if (y >= renderers->getFirst().get()->getGameObject()->getTransform()->getWorldPosition().y) {
			renderers->pushFront(renderer);
		} else {

			// CASE 3 : LIST HAS ELEMENTS AND RENDERER IS IN A RANDOM LAYER, NOT THE LAST
			bool foundSmallerY = false;

			auto itSmallerY = renderers->getIterator();

			FOR_LIST_COND(it, renderers, !foundSmallerY)
			{
				Renderer* otherRenderer = it.get();
				f32 otherY = otherRenderer->getGameObject()->getTransform()->getWorldPosition().y;

				if (y >= otherY) {
					foundSmallerY = true;
					itSmallerY = it;
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
	// renderers->pushBack(renderer);

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
	renderer->setDestroyed();
	Memory::free<Renderer>(renderer);
}
// ---------------------------------------------------------------------------

void Batch::internalRemoveRendererFromList(const Iterator *it, List<Renderer*> *list) {
	auto castedIt = it->cast<Renderer*>();
	list->remove(*castedIt);

	Renderer* renderer = (*castedIt).get();
	renderer->setIsAlreadyInBatch(false);

	if (!renderer->isStatic() && renderer->isAffectedByProjection()) {
		// renderer->setChunk(nullptr);
		mRenderEngine->getLayersData()->get(renderer->getLayer())->mDynamicObjectsCount--;
	}
}

// ---------------------------------------------------------------------------

void Batch::setChunk(Chunk *chunk) {
	mChunk = chunk;
};
Chunk* Batch::getChunk() {
	return mChunk;
};

// ---------------------------------------------------------------------------

} /* namespace DE */
