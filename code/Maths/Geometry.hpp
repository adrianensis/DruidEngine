#pragma once

#include "Core/ObjectBase.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"

CLASS(Shape, ObjectBase){};

CLASS(Rectangle, Shape){
    public :

};

class Geometry
{
public:
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