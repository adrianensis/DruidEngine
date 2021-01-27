#pragma once

#include "Scene/GameObject.hpp"
#include "Graphics/Material.hpp"
#include "Maths/Vector3.hpp"
#include "Scene/Scene.hpp"

namespace DE
{

    class MapElement : public GameObject {
    private:

    public:
        DE_CLASS(MapElement)

        virtual void init() override;

        static MapElement *create(Scene *scene, Vector3 position, const Vector2 &size, Material *material, u32 layer);
    };

}

