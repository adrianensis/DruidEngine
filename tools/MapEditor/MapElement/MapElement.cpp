#include "MapElement.hpp"

#include "Memory/Memory.hpp"
#include "Scene/Transform.hpp"
#include "Graphics/Renderer.hpp"
#include "Log/Log.hpp"

namespace DE
{
    MapElement::MapElement() : GameObject() {}
    MapElement::~MapElement() {}

    void MapElement::initFromData(const MapElementData* data) {
        getTransform()->setLocalPosition(data->mPosition);
        getTransform()->setScale(Vector3(data->mSize, 1));

        Renderer *renderer = DE_NEW<Renderer>();
        addComponent<Renderer>(renderer);

        renderer->setMesh(Mesh::getRectangle());
        renderer->setMaterial(data->mMaterial);

        renderer->setLayer(data->mLayer);

        setIsStatic(true);
        setShouldPersist(true);
    }

    MapElement* MapElement::create(const MapElementData* data) {
        MapElement *mapElement = DE_NEW_FROM_NAME<MapElement>(data->mMapElementClassName);
        mapElement->init();
        mapElement->initFromData(data);

        //mapElement->setTag("mapElement");

        return mapElement;
    }

    MapElement_Tile::MapElement_Tile() : MapElement() {}
    MapElement_Tile::~MapElement_Tile() {}

    MapElement_ActionPoint::MapElement_ActionPoint() : MapElement() {}
    MapElement_ActionPoint::~MapElement_ActionPoint() {}

} // namespace DE