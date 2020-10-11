#include "GameObjectBuilder.hpp"

#include "List.hpp"
#include "HashMap.hpp"

#include "Component.hpp"
#include "Transform.hpp"
#include "Material.hpp"
#include "Memory.hpp"
#include "Renderer.hpp"
#include "Mesh.hpp"

namespace DE {

// ---------------------------------------------------------------------------

GameObjectBuilder::GameObjectBuilder() : DE_Class(), Singleton<GameObjectBuilder>() {

}

GameObjectBuilder::~GameObjectBuilder() {

}

GameObjectBuilder* GameObjectBuilder::createSprite(Material* material, u32 layer, bool isStatic) {
	mGameObject = Memory::allocate<GameObject>();
	mGameObject->init();
	mGameObject->setIsStatic(isStatic);

	Renderer* renderer = Memory::allocate<Renderer>();
	mGameObject->addComponent<Renderer>(renderer);

	renderer->setLayer(layer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(material);
}

GameObject* GameObjectBuilder::getGameObject() {
	return mGameObject;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
