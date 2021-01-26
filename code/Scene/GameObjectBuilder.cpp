#include "Scene/GameObjectBuilder.hpp"

#include "Containers/List.hpp"
#include "Containers/HashMap.hpp"

#include "Scene/Component.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Material.hpp"
#include "Memory/Memory.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Mesh.hpp"

namespace DE {

// ---------------------------------------------------------------------------

GameObjectBuilder::GameObjectBuilder() : DE_Class(), Singleton<GameObjectBuilder>() {

}

GameObjectBuilder::~GameObjectBuilder() {

}

GameObjectBuilder* GameObjectBuilder::createSprite(Material* material, u32 layer, bool isStatic) {
	mGameObject = DE_NEW<GameObject>();
	mGameObject->init();
	mGameObject->setIsStatic(isStatic);

	Renderer* renderer = DE_NEW<Renderer>();
	mGameObject->addComponent<Renderer>(renderer);

	renderer->setLayer(layer);

	renderer->setMesh(Mesh::getRectangle());
	renderer->setMaterial(material);

	return this;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
