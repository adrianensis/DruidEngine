#pragma once

#include "Core/Macros.hpp"

#define VECTOR_PARAMETERS_PACK(vectorLength, objectName) VECTOR_PARAMETERS_PACK_##vectorLength(objectName)

#define VECTOR_PARAMETERS_PACK_2(objectName) objectName.x, objectName.y
#define VECTOR_PARAMETERS_PACK_3(objectName) objectName.x, objectName.y, objectName.z
#define VECTOR_PARAMETERS_PACK_4(objectName) objectName.x, objectName.y, objectName.z, objectName.w

#define VECTOR_BASE_DEFINITION(vectorLength) \
private:\
    void checkBoundaries(u32 index) CNS\
    {\
        ASSERT_MSG(index >= 0 && index < vectorLength, "Index out of bounds.");\
    }\
    f32 &get(u32 index)\
    {\
        checkBoundaries(index);\
        return *(&x + index);\
    }\
    CNS f32 &get(u32 index) CNS\
    {\
        checkBoundaries(index);\
        return *(&x + index);\
    }\
public:\
Vector##vectorLength &set(CNS Vector##vectorLength &rhs);\
Vector##vectorLength &add(CNS Vector##vectorLength &rhs);\
Vector##vectorLength &sub(CNS Vector##vectorLength &rhs);\
Vector##vectorLength &mul(CNS Vector##vectorLength &rhs);\
Vector##vectorLength &div(CNS Vector##vectorLength &rhs);\
Vector##vectorLength &add(f32 rhs);\
Vector##vectorLength &sub(f32 rhs);\
Vector##vectorLength &mul(f32 rhs);\
Vector##vectorLength &div(f32 rhs);\
\
f32 len() CNS;\
f32 sqrlen() CNS;\
f32 max() CNS;\
f32 min() CNS;\
f32 dot(CNS Vector##vectorLength &v) CNS;\
Vector##vectorLength &nor();\
f32 dst(CNS Vector##vectorLength &v) CNS;\
f32 sqrdst(CNS Vector##vectorLength &v) CNS;\
bool eq(CNS Vector##vectorLength &v, f32 e) CNS;\
bool eq(CNS Vector##vectorLength &v) CNS;\
Vector##vectorLength &lerp(CNS Vector##vectorLength &target, f32 t);\
f32 angle(CNS Vector##vectorLength &v) CNS;\
Vector##vectorLength &clamp(f32 maxLength);\
\
COPY(Vector##vectorLength){ set(*other); }\
\
Vector##vectorLength &operator+=(CNS Vector##vectorLength &rhs) {return this->add(rhs); }\
Vector##vectorLength &operator-=(CNS Vector##vectorLength &rhs) {return this->sub(rhs); }\
Vector##vectorLength &operator*=(CNS Vector##vectorLength &rhs) {return this->mul(rhs); }\
Vector##vectorLength &operator/=(CNS Vector##vectorLength &rhs) {return this->div(rhs); }\
Vector##vectorLength &operator+=(f32 rhs) {return this->add(rhs); }\
Vector##vectorLength &operator-=(f32 rhs) {return this->sub(rhs); }\
Vector##vectorLength &operator*=(f32 rhs) {return this->mul(rhs); }\
Vector##vectorLength &operator/=(f32 rhs) {return this->div(rhs); }\
bool operator==(CNS Vector##vectorLength &rhs) CNS {return this->eq(rhs); }\
bool operator!=(CNS Vector##vectorLength &rhs) CNS {return !((*this) == rhs); }\
Vector##vectorLength operator+(CNS Vector##vectorLength &rhs) CNS {return Vector##vectorLength(*this) += rhs; }\
Vector##vectorLength operator-(CNS Vector##vectorLength &rhs) CNS {return Vector##vectorLength(*this) -= rhs; }\
Vector##vectorLength operator*(CNS Vector##vectorLength &rhs) CNS {return Vector##vectorLength(*this) *= rhs; }\
Vector##vectorLength operator/(CNS Vector##vectorLength &rhs) CNS {return Vector##vectorLength(*this) /= rhs; }\
Vector##vectorLength operator+(f32 rhs) CNS {return Vector##vectorLength(*this) += rhs; }\
Vector##vectorLength operator-(f32 rhs) CNS {return Vector##vectorLength(*this) -= rhs; }\
Vector##vectorLength operator-() CNS{ return Vector##vectorLength(*this) *= -1; }\
Vector##vectorLength operator*(f32 rhs) CNS {return Vector##vectorLength(*this) *= rhs; }\
Vector##vectorLength operator/(f32 rhs) CNS {return Vector##vectorLength(*this) /= rhs; }\
f32 &operator[](CNS size_t index) { return get(index); }\
f32 operator[](CNS size_t index) CNS { return get(index); }\
SERIALIZE();\
DESERIALIZE();
