#include "MapElement.hpp"

#include "Memory/Memory.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Renderer.hpp"

namespace DE
{

    MapElement::MapElement() : GameObject() {

    }

    MapElement::~MapElement() {

    }

    void MapElement::init() {
        GameObject::init();
        /* TODO : all derived MapElements will
        create its specific components here.

        Maybe in another method? init is too soon?
        */
    }

    MapElement* MapElement::create(Scene* scene, Vector3 position, const Vector2 &size, Material *material, u32 layer) {
        MapElement *mapElement = Memory::allocate<MapElement>();
        mapElement->init();

        mapElement->getTransform()->setLocalPosition(position);
        mapElement->getTransform()->setScale(Vector3(size, 1));

        Renderer *renderer = Memory::allocate<Renderer>();
        mapElement->addComponent<Renderer>(renderer);

        renderer->setMesh(Mesh::getRectangle());
        renderer->setMaterial(material);

        renderer->setLayer(layer);

        mapElement->setIsStatic(true);
        mapElement->setShouldPersist(true);

        mapElement->setTag("mapElement");

        scene->addGameObject(mapElement);

        return mapElement;
    }

} // namespace DE