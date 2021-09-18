#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector2.hpp"

CLASS(Shape, ObjectBase)
{
public:

    SERIALIZE() { }
	DESERIALIZE() { }
};

CLASS(Rectangle, Shape)
{
    PRO(LeftTop, GETREF_CONST_SET, Vector2)
    PRO(Size, GETREF_CONST_SET, Vector2)

public:

    Rectangle() { }

    Rectangle(f32 x, f32 y, f32 w, f32 h)
    {
        mLeftTop.set(x,y);
        mSize.set(w, h);
    }

    Rectangle(CNS Vector2 &leftTop, f32 w, f32 h)
    {
        mLeftTop.set(leftTop);
        mSize.set(w, h);
    }

    Rectangle(CNS Vector2 &leftTop, CNS Vector2 &size)
    {
        mLeftTop.set(leftTop);
        mSize.set(size);
    }

    COPY(Rectangle)
    {
        DO_COPY(LeftTop)
        DO_COPY(Size)
    }

    SERIALIZE()
    {
        SUPER_SERIALIZE()

        DO_SERIALIZE("left_top", mLeftTop)
        DO_SERIALIZE("size", mSize)
    }

    DESERIALIZE()
    {
        SUPER_DESERIALIZE()

        DO_DESERIALIZE("left_top", mLeftTop);
        DO_DESERIALIZE("size", mSize);
    }
};

class Geometry
{
public:
    // Geometry tests
    STC bool testRectanglePoint(CNS Vector2 &leftTop, f32 width, f32 height, CNS Vector2 &point, f32 eps);
    STC bool testRectangleSphere(CNS Vector2 &leftTop, f32 width, f32 height, CNS Vector2 &center, f32 radius, f32 eps);
    STC bool testSphereSphere(CNS Vector2 &centerA, CNS Vector2 &centerB, f32 radiusA, f32 radiusB, f32 eps);
    STC bool testLineLine(CNS Vector2 &lineAStart, CNS Vector2 &lineAEnd, CNS Vector2 &lineBStart, CNS Vector2 &lineBEnd, Vector2 &intersectionResult);
    STC bool testLineSphereSimple(CNS Vector2 &lineStart, CNS Vector2 &lineEnd, CNS Vector2 &center, f32 radius, f32 eps);
    STC bool testLineSphere(CNS Vector2 &lineStart, CNS Vector2 &lineEnd, CNS Vector2 &center, f32 radius, f32 eps, Vector2 &intersectionResult1, Vector2 &intersectionResult2);
    STC bool testSpherePoint(CNS Vector2 &point, CNS Vector2 &center, f32 radius);
    STC bool testLinePoint(CNS Vector2 &lineStart, CNS Vector2 &lineEnd, CNS Vector2 &point, f32 eps);
    STC Vector2 closestPointInLine(CNS Vector2 &lineStart, CNS Vector2 &lineEnd, CNS Vector2 &point);
    STC Vector2 midPoint(CNS Vector2 &a, CNS Vector2 &b);
};