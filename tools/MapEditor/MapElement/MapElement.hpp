#pragma once

#include "Scene/GameObject.hpp"
#include "Graphics/Material.hpp"
#include "Maths/Vector3.hpp"
#include "Scene/Scene.hpp"

namespace DE
{
    class MapElementData;
    
    class MapElement : public GameObject {
    public:
        DE_CLASS(MapElement)
        virtual void initFromData(const MapElementData* data);
        static MapElement *create(const MapElementData* data);
    };

    class MapElementData : public DE_Class {
    public:
        DE_GENERATE_METADATA(MapElementData)

        DE_M(MapElementClassName, std::string);
        DE_M(Position, Vector3);
        DE_M(Size, Vector2);
        DE_M(Material, Material*);
        DE_M(Layer, u32);
        DE_M(MaterialRegionPosition, Vector2);
        DE_M(MaterialRegionSize, Vector2);

        MapElementData() { mMapElementClassName = MapElement::getClassNameStatic(); }
    };

    class MapElement_Tile : public MapElement {
    public:
        DE_CLASS(MapElement_Tile)
    };

    class MapElementData_Tile : public MapElementData {
    public:
        DE_GENERATE_METADATA(MapElementData_Tile)
        MapElementData_Tile() { mMapElementClassName = MapElement_Tile::getClassNameStatic(); }
    };

    class MapElement_ActionPoint : public MapElement {
    public:
        DE_CLASS(MapElement_ActionPoint)
    };

    class MapElementData_ActionPoint : public MapElementData {
    public:
        DE_GENERATE_METADATA(MapElementData_ActionPoint)
        MapElementData_ActionPoint() { mMapElementClassName = MapElement_ActionPoint::getClassNameStatic(); }
    };
}

