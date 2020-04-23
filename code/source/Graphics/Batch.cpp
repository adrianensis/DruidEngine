#include "Batch.h"
#include "Material.h"
#include "Texture.h"
#include "List.h"
#include "Renderer.h"
#include "RenderEngine.h"
#include "Camera.h"
#include "Frustum.h"
#include "Mesh.h"
#include "Shader.h"
#include "RenderContext.h"
#include "List.h"
#include "HashMap.h"
#include "Log.h"
#include "GameObject.h"
#include "Transform.h"
#include "MathUtils.h"
#include "Time.h"
#include "Chunk.h"
#include "Settings.h"

namespace DE {

Matrix4 Batch::smMatrixIdentity;

// ---------------------------------------------------------------------------

u8 Batch::rendererYCoordinateComparator(Renderer* a, Renderer* b){

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

Batch::LayerData::LayerData() : DE_Class(){
	mSorted = false;
	mDynamicObjectsCount = 0;
	mSortCounter = 0;
}

// ---------------------------------------------------------------------------

Batch::LayerData::~LayerData() = default;

// ---------------------------------------------------------------------------

Batch::Batch() : DE_Class(){
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

	mSortByYCoordinate = false;
}

Batch::~Batch(){

	FOR_LIST(itList, mRenderers->getValues()){
		if(itList.get()){
			FOR_LIST(itRenderer, itList.get()){
				if(! itRenderer.get()->isDestroyed()){
					itRenderer.get()->setDestroyed();
					Memory::free<Renderer>(itRenderer.get());
				}
			}

	    Memory::free<List<Renderer*>>(itList.get());
		}
  }

	Memory::free<HashMap<u32, List<Renderer*>*>>(mRenderers);

	FOR_LIST(it, mLayersData->getValues()){
		Memory::free<LayerData>(it.get());
	}

	Memory::free<HashMap<u32, LayerData*>>(mLayersData);

	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBOPosition);
	glDeleteBuffers(1, &mEBO);
}

// ---------------------------------------------------------------------------

void Batch::init(const Mesh* mesh, Material* material){
	// TRACE();

	mSortByYCoordinate = Settings::getInstance()->getBool("scene.sortByYCoordinate");

	mRenderEngine = RenderEngine::getInstance();

	mRenderers = Memory::allocate<HashMap<u32, List<Renderer*>*>>();
	mRenderers->init();

	mLayersData = Memory::allocate<HashMap<u32, LayerData*>>();
	mLayersData->init();

	u32 maxLayers = 10; // MOVE TO SETTINGS
	FOR_RANGE(i, 0, 10){
		mRenderers->set(i, nullptr);
		mLayersData->set(i, Memory::allocate<LayerData>());
	}


	mMesh = mesh;
	mMaterial = material;

	smMatrixIdentity.identity();

	bind();
}

// ---------------------------------------------------------------------------

void Batch::bind(){
	mVAO = RenderContext::createVAO();
	mVBOPosition = RenderContext::createVBO(mMesh->getVertices(), 3, 0);
	mVBOTexture = RenderContext::createVBO(mMesh->getTextureCoordinates(), 2, 1);
	//mVBOColor = RenderContext::createVBO(mMesh->getColors(), 4, 2);
//	mVBONormal = RenderContext::createVBO(mMesh->getNormals(), 3, 3);
	mEBO = RenderContext::createEBO(mMesh->getFaces());

	glGenTextures(1, &mTextureId);

	glBindTexture(GL_TEXTURE_2D, mTextureId);

	Texture* texture = mMaterial->getTexture();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->getWidth(), texture->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->getData());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// Prevents s-coordinate wrapping (repeating).
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// Prevents t-coordinate wrapping (repeating).
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	RenderContext::enableVAO(0);
}

// ---------------------------------------------------------------------------

void Batch::update(){

}

// ---------------------------------------------------------------------------

bool Batch::checkInFrustum(Camera* cam, Renderer* renderer){
	Transform* t = renderer->getGameObject()->getTransform();

	Vector3 scale = t->getScale();
	f32 maxRadius = std::max(scale.x, scale.y); // TODO: if 3D, compare also with z

	Vector3 position(Vector3(t->getWorldPosition()).add(renderer->getPositionOffset()));

	return cam->getFrustum()->testSphere(position, maxRadius);
}

// ---------------------------------------------------------------------------

bool Batch::checkDistance(Camera* cam, Renderer* renderer){
	Transform* t = renderer->getGameObject()->getTransform();

	Vector3 camPosition(cam->getGameObject()->getTransform()->getLocalPosition());
	Vector3 rendererPosition(t->getLocalPosition());

	return rendererPosition.dst(camPosition) < renderer->getRenderDistance();
}

// ---------------------------------------------------------------------------

bool Batch::checkOutOfCamera(Camera* cam, Renderer* renderer){

	bool isOutOfCamera = false;

	if(renderer->isAffectedByProjection()){
		renderer->setIsOutOfCamera(!checkInFrustum(cam, renderer));
		isOutOfCamera = renderer->isOutOfCamera();
	} else {
		isOutOfCamera = false;
	}

	return isOutOfCamera;
}

// ---------------------------------------------------------------------------

u32 Batch::render(u32 layer){

	u32 drawCallCounter = 0;

	List<Renderer*>* renderers = mRenderers->get(layer);

	if(renderers && renderers->getLength() > 0){

		Shader* shader = mMaterial->getShader();

		shader->use();
		RenderContext::enableVAO(mVAO);
		glBindTexture(GL_TEXTURE_2D, mTextureId);

		Camera* camera = mRenderEngine->getCamera();

	  const Matrix4& projectionMatrix = camera->getProjectionMatrix();
	  const Matrix4& viewTranslationMatrix = camera->getViewTranslationMatrix();
	  const Matrix4& viewRotationMatrix = camera->getViewRotationMatrix();

	  shader->addMatrix(projectionMatrix, "projectionMatrix");
	  shader->addMatrix(viewTranslationMatrix, "viewTranslationMatrix");
	  shader->addMatrix(viewRotationMatrix, "viewRotationMatrix");

		FOR_LIST(it, renderers){

			Renderer* renderer = it.get();

			Chunk* chunk = renderer->getChunk();
			bool chunkOk = (! chunk) || (chunk && chunk->isLoaded());

			if(renderer->isActive() && chunkOk){

				Transform* t = renderer->getGameObject()->getTransform();

				if(renderer->getLayer() == layer && !checkOutOfCamera(camera,renderer)){

					const Matrix4& translationMatrix = t->getTranslationMatrix();
					const Matrix4& rotationMatrix = t->getRotationMatrix();
					const Matrix4& scaleMatrix = t->getScaleMatrix();

					shader->addMatrix(translationMatrix, "translationMatrix");
					shader->addMatrix(renderer->getPositionOffsetMatrix(), "positionOffsetMatrix");
					shader->addMatrix(rotationMatrix, "rotationMatrix");
					shader->addMatrix(scaleMatrix, "scaleMatrix");

					if(! renderer->isAffectedByProjection()){
						shader->addMatrix(smMatrixIdentity, "projectionMatrix");
						shader->addMatrix(smMatrixIdentity, "viewTranslationMatrix");
					 	shader->addMatrix(smMatrixIdentity, "viewRotationMatrix");
					}

					shader->addFloat(Time::getDeltaTimeSeconds(), "time");

					renderer->updateMaterial(mMaterial);

					bool lineMode = it.get()->isLineMode();

					glPolygonMode(GL_FRONT_AND_BACK, lineMode ? GL_LINE : GL_FILL);

					glDrawElements(GL_TRIANGLES, mMesh->getFaces()->getLength(), GL_UNSIGNED_INT, 0);

					drawCallCounter++;

				} else if(renderer->isPendingToBeDestroyed()){
						// destroy renderer and remove from list
						internalRemoveRenderer(&it, renderers);
				}
			} else if (renderer->isAffectedByProjection()){
				if(! chunk->isLoaded()){
					internalRemoveRendererFromList(&it, renderers);
				}
			}

			if(mSortByYCoordinate && renderer->isAffectedByProjection() && ! renderer->isStatic()){
				internalRemoveRendererFromList(&it, renderers);
			}

		}

		RenderContext::enableVAO(0);
	}

	return drawCallCounter;
}

// ---------------------------------------------------------------------------

void Batch::insertSorted(Renderer* renderer, List<Renderer*>* renderers){

	// INSERT SORTED

	if(mSortByYCoordinate){

			f32 y = renderer->getGameObject()->getTransform()->getWorldPosition().y;

			// CASE 1 : IF LIST IS EMPTY
			if(renderers->isEmpty()){
				renderers->pushBack(renderer);
			} else {


				// CASE 2 : RENDERER IS IN THE LAST/FIRST LAYER
				if(y <= renderers->getLast().get()->getGameObject()->getTransform()->getWorldPosition().y){
					renderers->pushBack(renderer);
				} else if(y >= renderers->getFirst().get()->getGameObject()->getTransform()->getWorldPosition().y){
					renderers->pushFront(renderer);
				} else {

					// CASE 3 : LIST HAS ELEMENTS AND RENDERER IS IN A RANDOM LAYER, NOT THE LAST
					bool foundSmallerY = false;

					auto itSmallerY = renderers->getIterator();

					FOR_LIST_COND(it, renderers, !foundSmallerY){
						Renderer* otherRenderer = it.get();
						f32 otherY = otherRenderer->getGameObject()->getTransform()->getWorldPosition().y;

						if(y >= otherY){
							foundSmallerY = true;
							itSmallerY = it;
						}
					}

					if(foundSmallerY){
						renderers->insert(itSmallerY, renderer); // this method inserts before the iterator
					}else{
						renderers->pushBack(renderer);
					}
				}
			}
	}else{
		renderers->pushBack(renderer);
	}
}

// ---------------------------------------------------------------------------

void Batch::addRenderer(Renderer* renderer){

		checkOutOfCamera(mRenderEngine->getCamera(),renderer);

		u32 layer = renderer->getLayer();

		List<Renderer*>* renderers = mRenderers->get(layer);

		if(!renderers){
			renderers = Memory::allocate<List<Renderer*>>();
			renderers->init();

			mRenderers->set(layer, renderers);
		}

		renderer->setIsAlreadyInBatch(true);
		// renderers->pushBack(renderer);

		if(!renderer->isStatic() && renderer->isAffectedByProjection()){
			mLayersData->get(renderer->getLayer())->mDynamicObjectsCount++;
		}

		insertSorted(renderer, renderers);
}

// ---------------------------------------------------------------------------

void Batch::internalRemoveRenderer(const Iterator* it, List<Renderer*>* list){
	internalRemoveRendererFromList(it, list);

	auto castedIt = it->cast<Renderer*>();
	Renderer* renderer = (*castedIt).get();
	renderer->setDestroyed();
	Memory::free<Renderer>(renderer);
}
// ---------------------------------------------------------------------------

void Batch::internalRemoveRendererFromList(const Iterator* it, List<Renderer*>* list){
	auto castedIt = it->cast<Renderer*>();
	list->remove(*castedIt);

	Renderer* renderer = (*castedIt).get();
	renderer->setIsAlreadyInBatch(false);

	if(!renderer->isStatic()){
		// renderer->setChunk(nullptr);
		mLayersData->get(renderer->getLayer())->mDynamicObjectsCount--;
	}
}

// ---------------------------------------------------------------------------

void Batch::setChunk(Chunk* chunk ){ mChunk = chunk; };
Chunk* Batch::getChunk(){ return mChunk; };

// ---------------------------------------------------------------------------

} /* namespace DE */
