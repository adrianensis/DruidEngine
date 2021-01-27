#include "Maths/Vector2.hpp"
#include "Maths/Vector3.hpp"

#include <algorithm>  // std::max

namespace DE {

Vector2::Vector2(f32 x, f32 y) : DE_Class(), x(x), y(y) {
}

Vector2::Vector2() : DE_Class(), x(0.0f), y(0.0f) {
}

Vector2::~Vector2() = default;

Vector2::Vector2(const Vector2 &other) : DE_Class(), x(other.x), y(other.y) {
}

Vector2::Vector2(const Vector3 &other) : DE_Class(), x(other.x), y(other.y) {
}

Vector2& Vector2::set(f32 x, f32 y) {
	if (this->x == x && this->y == y)
		return *this; // handle self assignment
	//assignment operator
	this->x = x;
	this->y = y;
	return *this;
}

Vector2& Vector2::set(const Vector2 &rhs) {
	if (this == &rhs)
		return *this; // handle self assignment
	//assignment operator
	this->set(rhs.x, rhs.y);
	return *this;
}

Vector2& Vector2::add(const Vector2 &rhs) {
	// can be parallelized with SIMD auto-vectorization
	x = x + rhs.x;
	y = y + rhs.y;
	return *this;
}

Vector2& Vector2::sub(const Vector2 &rhs) {
	x = x - rhs.x;
	y = y - rhs.y;
	return *this;
}

Vector2& Vector2::mul(const Vector2 &rhs) {
	x = x * rhs.x;
	y = y * rhs.y;
	return *this;
}

Vector2& Vector2::div(const Vector2 &rhs) {
	DE_ASSERT(rhs.x != 0, "Division by zero.");
	DE_ASSERT(rhs.y != 0, "Division by zero.");
	x = x / rhs.x;
	y = y / rhs.y;
	return *this;
}

Vector2& Vector2::add(f32 rhs) {
	x = x + rhs;
	y = y + rhs;
	return *this;
}

Vector2& Vector2::sub(f32 rhs) {
	x = x - rhs;
	y = y - rhs;
	return *this;
}

Vector2& Vector2::mul(f32 rhs) {
	x = x * rhs;
	y = y * rhs;
	return *this;
}

Vector2& Vector2::div(f32 rhs) {
	DE_ASSERT(rhs != 0, "Division by zero.");
	x = x / rhs;
	y = y / rhs;
	return *this;
}

f32 Vector2::dot(const Vector2 &v) const {
	return this->x * v.x + this->y * v.y;
}

f32 Vector2::sqrlen() const {
	return this->dot(*this);
}

f32 Vector2::sqrdst(const Vector2 &v) const {
	Vector2 sub = DE::Vector2(v) - (*this);
	return sub.dot(sub);
}

f32 Vector2::len() const {
	return sqrtf(this->sqrlen());
}

f32 Vector2::max() const {
	return std::max(x, y);
}

f32 Vector2::min() const {
	return std::min(x, y);
}

Vector2& Vector2::nor() {
	f32 len = this->len();

	DE_ASSERT(len > 0, "Length is zero.");
	this->div(len);

	return *this;
}

f32 Vector2::dst(const Vector2 &v) const {
	return sqrtf(this->sqrdst(v));
}

bool Vector2::eq(const Vector2 &v) const {
	return MathUtils::eqf(this->x, v.x) && MathUtils::eqf(this->y, v.y);
}

bool Vector2::eq(const Vector2 &v, f32 e) const {
	return MathUtils::eqf(this->x, v.x, e) && MathUtils::eqf(this->y, v.y, e);
}

Vector2& Vector2::lerp(const Vector2 &target, f32 t) {
	//start + percent*(end - start)
	(*this) += (Vector2(target) - (*this)) * t;
	return *this;
}

f32 Vector2::angle(const Vector2 &v) const {
	f32 angle = atan2f(v.y, v.x) - atan2f(this->y, this->x);
	return angle < 0 ? angle += 2 * MathUtils::PI : angle;
}

Vector2& Vector2::clamp(f32 maxLength) {
	if (this->sqrlen() > maxLength * maxLength) {
		this->nor();
		this->mul(maxLength);
	}

	return *this;
}

}
