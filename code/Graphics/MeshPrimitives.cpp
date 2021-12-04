#include "Graphics/MeshPrimitives.hpp"

void MeshPrimitives::init()
{

}

void MeshPrimitives::terminate()
{
	MAP_DELETE_CONTENT(mPrimitivesMap);
}

template <>
const Mesh* MeshPrimitives::createPrimitive<Rectangle2D>() const
{
	Mesh* mesh = NEW(Mesh);
	mesh->init(4, 2);

	mesh->
	addVertex(Vector3(-0.5f, 0.5f, 0.0f))-> // top left
	addVertex(Vector3(-0.5f, -0.5f, 0.0f))-> // bottom left
	addVertex(Vector3(0.5f, -0.5f, 0.0f))-> // bottom right
	addVertex(Vector3(0.5f, 0.5f, 0.0f))-> // top right

	addTexCoord(0.0f, 1.0f)-> // top left
	addTexCoord(0.0f, 0.0f)-> // bottom left
	addTexCoord(1.0f, 0.0f)-> // bottom right
	addTexCoord(1.0f, 1.0f)-> // top right

	addFace(0, 1, 3)->
	addFace(1, 2, 3);

	return mesh;
}