#pragma once

#include "Core/Macros.hpp"

#define VECTOR_BASE_DEFINITION(vectorLength) \
PRI\
    void checkBoundaries(u32 index) const\
    {\
        ASSERT_MSG(index >= 0 && index < vectorLength, "Index out of bounds.");\
    }\
    f32 &get(u32 index)\
    {\
        checkBoundaries(index);\
        return *(&x + index);\
    }\
    const f32 &get(u32 index) const\
    {\
        checkBoundaries(index);\
        return *(&x + index);\
    }\
PUB\
    Vector##vectorLength &set(const Vector##vectorLength &rhs);\
    Vector##vectorLength &add(const Vector##vectorLength &rhs);\
    Vector##vectorLength &sub(const Vector##vectorLength &rhs);\
    Vector##vectorLength &mul(const Vector##vectorLength &rhs);\
    Vector##vectorLength &div(const Vector##vectorLength &rhs);\
    Vector##vectorLength &add(f32 rhs);\
    Vector##vectorLength &sub(f32 rhs);\
    Vector##vectorLength &mul(f32 rhs);\
    Vector##vectorLength &div(f32 rhs);\
    \
    f32 len() const;\
    f32 sqrlen() const;\
    f32 max() const;\
    f32 min() const;\
    f32 dot(const Vector##vectorLength &v) const;\
    Vector##vectorLength &nor();\
    f32 dst(const Vector##vectorLength &v) const;\
    f32 sqrdst(const Vector##vectorLength &v) const;\
    bool eq(const Vector##vectorLength &v, f32 e) const;\
    bool eq(const Vector##vectorLength &v) const;\
    Vector##vectorLength &lerp(const Vector##vectorLength &target, f32 t);\
    f32 angle(const Vector##vectorLength &v) const;\
    Vector##vectorLength &clamp(f32 maxLength);\
    \
    Vector##vectorLength &operator=(const Vector##vectorLength &other)\
	{\
		set(other);\
		return *this;\
	}\
    \
    Vector##vectorLength &operator+=(const Vector##vectorLength &rhs) {return this->add(rhs); }\
    Vector##vectorLength &operator-=(const Vector##vectorLength &rhs) {return this->sub(rhs); }\
    Vector##vectorLength &operator*=(const Vector##vectorLength &rhs) {return this->mul(rhs); }\
    Vector##vectorLength &operator/=(const Vector##vectorLength &rhs) {return this->div(rhs); }\
    Vector##vectorLength &operator+=(f32 rhs) {return this->add(rhs); }\
    Vector##vectorLength &operator-=(f32 rhs) {return this->sub(rhs); }\
    Vector##vectorLength &operator*=(f32 rhs) {return this->mul(rhs); }\
    Vector##vectorLength &operator/=(f32 rhs) {return this->div(rhs); }\
    bool operator==(const Vector##vectorLength &rhs) const {return this->eq(rhs); }\
    bool operator!=(const Vector##vectorLength &rhs) const {return !((*this) == rhs); }\
    Vector##vectorLength operator+(const Vector##vectorLength &rhs) const {return Vector##vectorLength(*this) += rhs; }\
    Vector##vectorLength operator-(const Vector##vectorLength &rhs) const {return Vector##vectorLength(*this) -= rhs; }\
    Vector##vectorLength operator*(const Vector##vectorLength &rhs) const {return Vector##vectorLength(*this) *= rhs; }\
    Vector##vectorLength operator/(const Vector##vectorLength &rhs) const {return Vector##vectorLength(*this) /= rhs; }\
    Vector##vectorLength operator+(f32 rhs) const {return Vector##vectorLength(*this) += rhs; }\
    Vector##vectorLength operator-(f32 rhs) const {return Vector##vectorLength(*this) -= rhs; }\
    Vector##vectorLength operator-() const{ return Vector##vectorLength(*this) *= -1; }\
    Vector##vectorLength operator*(f32 rhs) const {return Vector##vectorLength(*this) *= rhs; }\
    Vector##vectorLength operator/(f32 rhs) const {return Vector##vectorLength(*this) /= rhs; }\
    f32 &operator[](const size_t index) { return get(index); }\
    f32 operator[](const size_t index) const { return get(index); }
