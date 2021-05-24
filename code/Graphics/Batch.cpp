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

	mRenderEngine = RenderEngine::getInstance();
	mMesh = mesh;
	mMaterial = material;
	mIsWorldSpace = true;

	mMaxMeshes = 2000;
	mMeshesIndex = 0;

	mMeshBuilder.init(mesh->getVertexCount() * mMaxMeshes, mesh->getFacesCount() * mMaxMeshes);

	FOR_RANGE(i, 0, mRenderEngine->getMaxLayers()) {
		MAP_INSERT(mRenderers, i, nullptr)
	}

	bind();
}

void Batch::bind() {
	mVAO = RenderContext::createVAO();
	mVBOPosition = RenderContext::createVBO(Mesh::smVertexPositionSize, 0);
	mVBOTexture = RenderContext::createVBO(Mesh::smVertexTexCoordSize, 1);
	mVBOColor = RenderContext::createVBO(Mesh::smVertexColorSize, 2);
	//mVBONormal = RenderContext::createVBO(mMesh->getNormals(), 3, 3);
	mEBO = RenderContext::createEBO();

	FOR_RANGE(i, 0, mMaxMeshes) {
		i32 offset = + (4*i);
		mMeshBuilder.addFace(0 + offset, 1 + offset, 3 + offset)->
		addFace(1 + offset, 2 + offset, 3 + offset);
	}

	RenderContext::setDataEBO(mEBO, mMeshBuilder.getFaces());

	Texture* texture = mMaterial->getTexture();

	if(texture) {
		texture->bind();
	}

	RenderContext::enableVAO(0);
}

void Batch::update() {

}

void Batch::render(u32 layer) {

	clearVertexBuffer();

	std::list<Renderer*>* renderers = mRenderers[layer];

	if (renderers && !renderers->empty()) {

		//bool isSortedLayer = mRenderEngine->getLayersData().at(layer)->mSorted;

		RenderContext::enableVAO(mVAO);

		mMaterial->bind(mIsWorldSpace);

		processRenderers(renderers);

		drawCall();

		RenderContext::enableVAO(0);
	}
}

void Batch::processRenderers(std::list<Renderer*>* renderers) {
	FOR_LIST(it, *renderers) {
		Renderer* renderer = *it;

		bool toRemove = false;

		if (renderer->getIsPendingToBeDestroyed()) {
			toRemove = true;
		} else if(renderer->isActive()) {
			if (isChunkOk(renderer)) {
				//if (!checkIsOutOfCamera(camera, renderer)) { }
				renderer->updateAnimation();
				addToVertexBuffer(renderer);
			} else {
				toRemove = true;
			}

			/*if (isSortedLayer && mIsWorldSpace && !renderer->isStatic()) {
				toRemove = true;
			}*/
		}

		if(toRemove){
			internalRemoveRendererFromList(it, renderers);
		}
	}
}

bool Batch::isChunkOk(Renderer* renderer) const {
	const Chunk* chunk = renderer->getChunk();
	return (!chunk) || (chunk && chunk->getIsLoaded()); // !chunk means -> Screen Space case
}

void Batch::drawCall() const {
	if(mMeshesIndex > 0) {
		RenderContext::setDataVBO(mVBOPosition, mMeshBuilder.getVertices());
		RenderContext::setDataVBO(mVBOTexture, mMeshBuilder.getTextureCoordinates());
		RenderContext::setDataVBO(mVBOColor, mMeshBuilder.getColors());

		RenderContext::drawRectangles(mMeshesIndex);
	}
}

void Batch::insertSorted(Renderer *renderer, std::list<Renderer*> *renderers) {

	// INSERT SORTED

	f32 y = renderer->getGameObject()->getTransform()->getWorldPosition().y;

	renderers->push_back(renderer);

	// CASE 1 : IF LIST IS EMPTY
	/*if (renderers->empty()) {
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

			renderers->push_back(renderer);
		}
	}*/
}

void Batch::addRenderer(Renderer *renderer) {

	u32 layer = renderer->getLayer();

	std::list<Renderer*>* renderers = mRenderers[layer];

	if (!renderers) {
		renderers = new std::list<Renderer*>;

		MAP_INSERT(mRenderers, layer, renderers);
	}

	if (!renderer->isStatic()) {
		mRenderEngine->getLayersData().at(renderer->getLayer())->mDynamicObjectsCount++;
	}

	if(mRenderEngine->getLayersData().at(renderer->getLayer())->mSorted){
		insertSorted(renderer, renderers);
	} else {
		renderers->push_back(renderer);
	}

	renderer->setIsAlreadyInBatch(true);

}

void Batch::internalRemoveRendererFromList(std::list<Renderer*>::iterator &it, std::list<Renderer*> *list) {
	Renderer* renderer = *it;

	renderer->setIsAlreadyInBatch(false);

	if(mIsWorldSpace) {
		if (!renderer->isStatic()) {
			mRenderEngine->getLayersData().at(renderer->getLayer())->mDynamicObjectsCount--;
		}
	} else {
		// NOTE: UI CASE
		// UI is not deleted in Chunk so it has to be deleted here.
		renderer->finallyDestroy();
		delete renderer;
	}

	it = list->erase(it);
}

void Batch::addToVertexBuffer(Renderer* renderer) {

	Transform* t = renderer->getGameObject()->getTransform();

	const std::vector<Vector2>* vertexPositions = renderer->getVertices();

	FOR_RANGE(i,0,mMesh->getVertexCount()) {

		mMeshBuilder.addVertex(vertexPositions->at(i));

		Vector2 vertexTexture(
		mMesh->getTextureCoordinates()[i*Mesh::smVertexTexCoordSize + 0],
		mMesh->getTextureCoordinates()[i*Mesh::smVertexTexCoordSize + 1]);

		Vector2 regionSize = renderer->getRegionSize();
		Vector2 regionPosition = renderer->getRegionPosition();

		Vector2 textureCoord(vertexTexture.x*regionSize.x + regionPosition.x, vertexTexture.y*regionSize.y + regionPosition.y);

		if(renderer->getInvertAxisX()){

			textureCoord.x = 1.0f - textureCoord.x;

			const Animation* animation = renderer->getCurrentAnimation();

			if(animation) {
				textureCoord.x = textureCoord.x - (1.0f - (animation->getNumberOfFrames()*regionSize.x));
			}
		}

		mMeshBuilder.addTexCoord(textureCoord.x, textureCoord.y);

		mMeshBuilder.addColor(
			renderer->getColor()[0],
			renderer->getColor()[1],
			renderer->getColor()[2],
			renderer->getColor()[3]);
	}

	mMeshesIndex++;
}

void Batch::clearVertexBuffer() {

	mMeshesIndex = 0;

	mMeshBuilder.clear();
}