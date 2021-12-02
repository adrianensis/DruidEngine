#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"

class Shape: public ObjectBase
{
    GENERATE_METADATA(Shape)
    
    PRO u32 mVerticesCount = 0; GET(VerticesCount)
PUB
    virtual void serialize(JSON &json) const override { }
	virtual void deserialize(const JSON &json) override { }

    COPY(Shape)
    {
        DO_COPY(mVerticesCount)
    }
};

class Line: public Shape
{
    GENERATE_METADATA(Line)
    PRO Vector3 mStart; GETREF_CONST_SET(Start)
    PRO Vector3 mEnd; GETREF_CONST_SET(End)

PUB
    Line() { mVerticesCount = 2; }

    Line(f32 xStart, f32 yStart, f32 xEnd, f32 yEnd): Line()
    {
        mStart.set(xStart,yStart, 0);
        mEnd.set(xEnd, yEnd, 0);
    }

    Line(const Vector3 &start, const Vector3 &end): Line()
    {
        mStart.set(start);
        mEnd.set(end);
    }

    COPY(Line)
    {
        Shape::copy(other);
        DO_COPY(mStart)
        DO_COPY(mEnd)
    }

    virtual void serialize(JSON &json) const override
    {
        Shape::serialize(json);

        DO_SERIALIZE("start", mStart)
        DO_SERIALIZE("end", mEnd)
    }

    virtual void deserialize(const JSON &json) override
    {
        Shape::deserialize(json);

        DO_DESERIALIZE("start", mStart)
        DO_DESERIALIZE("end", mEnd)
    }
};

class Rectangle: public Shape
{
    GENERATE_METADATA(Rectangle)
    PRO Vector2 mLeftTop; GETREF_CONST_SET(LeftTop)
    PRO Vector2 mSize; GETREF_CONST_SET(Size)

PUB
    Rectangle() { mVerticesCount = 4; }

    Rectangle(f32 x, f32 y, f32 w, f32 h): Rectangle()
    {
        mLeftTop.set(x,y);
        mSize.set(w, h);
    }

    Rectangle(const Vector2 &leftTop, f32 w, f32 h): Rectangle()
    {
        mLeftTop.set(leftTop);
        mSize.set(w, h);
    }

    Rectangle(const Vector2 &leftTop, const Vector2 &size): Rectangle()
    {
        mLeftTop.set(leftTop);
        mSize.set(size);
    }

    COPY(Rectangle)
    {
        Shape::copy(other);
        DO_COPY(mLeftTop)
        DO_COPY(mSize)
    }

    virtual void serialize(JSON &json) const override
    {
        Shape::serialize(json);

        DO_SERIALIZE("left_top", mLeftTop)
        DO_SERIALIZE("size", mSize)
    }

    virtual void deserialize(const JSON &json) override
    {
        Shape::deserialize(json);

        DO_DESERIALIZE("left_top", mLeftTop);
        DO_DESERIALIZE("size", mSize);
    }
};

class Geometry
{
PUB
    // Geometry tests
    static bool testRectanglePoint(const Vector2 &leftTop, f32 width, f32 height, const Vector2 &point, f32 eps);
    static bool testRectangleSphere(const Vector2 &leftTop, f32 width, f32 height, const Vector2 &center, f32 radius, f32 eps);
    static bool testSphereSphere(const Vector2 &centerA, const Vector2 &centerB, f32 radiusA, f32 radiusB, f32 eps);
    static bool testLineLine(const Vector2 &lineAStart, const Vector2 &lineAEnd, const Vector2 &lineBStart, const Vector2 &lineBEnd, Vector2 &intersectionResult);
    static bool testLineSphereSimple(const Vector2 &lineStart, const Vector2 &lineEnd, const Vector2 &center, f32 radius, f32 eps);
    static bool testLineSphere(const Vector2 &lineStart, const Vector2 &lineEnd, const Vector2 &center, f32 radius, f32 eps, Vector2 &intersectionResult1, Vector2 &intersectionResult2);
    static bool testSpherePoint(const Vector2 &point, const Vector2 &center, f32 radius);
    static bool testLinePoint(const Vector2 &lineStart, const Vector2 &lineEnd, const Vector2 &point, f32 eps);
    static Vector2 closestPointInLine(const Vector2 &lineStart, const Vector2 &lineEnd, const Vector2 &point);
    static Vector2 midPoint(const Vector2 &a, const Vector2 &b);
};