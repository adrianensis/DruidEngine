#include "Vector4.h"

#include <algorithm>  // std::max

namespace DE {

Vector4::Vector4(const f32 x, const f32 y, const f32 z, const f32 w):x(x),y(y),z(z),w(w){
}

Vector4::Vector4():x(0.0f),y(0.0f),z(0.0f),w(0.0f){
}

Vector4::~Vector4() = default;

Vector4::Vector4(const Vector4& other):x(other.x),y(other.y),z(other.z),w(other.w){
}

Vector4& Vector4::set(const f32 x, const f32 y, const f32 z, const f32 w) {
	if (this->x == x && this->y == y && this->z == z && this->w == w ) return *this; // handle self assignment
	//assignment operator
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
	return *this;
}

Vector4& Vector4::set(const Vector4& rhs) {
	if (this == &rhs) return *this; // handle self assignment
	//assignment operator
	this->set(rhs.x,rhs.y,rhs.z, rhs.w);
	return *this;
}

Vector4& Vector4::add(const Vector4& rhs) {
	// can be parallelized with SIMD auto-vectorization
	x = x + rhs.x;
	y = y + rhs.y;
	z = z + rhs.z;
	w = w + rhs.w;
	return *this;
}

Vector4& Vector4::sub(const Vector4& rhs) {
	x = x - rhs.x;
	y = y - rhs.y;
	z = z - rhs.z;
	w = w - rhs.w;
	return *this;
}

Vector4& Vector4::mul(const Vector4& rhs) {
	x = x * rhs.x;
	y = y * rhs.y;
	z = z * rhs.z;
	w = w * rhs.w;
	return *this;
}

Vector4& Vector4::div(const Vector4& rhs) {
	DE_ASSERT(rhs.x != 0, "Division by zero.");
	DE_ASSERT(rhs.y != 0, "Division by zero.");
	DE_ASSERT(rhs.z != 0, "Division by zero.");
	DE_ASSERT(rhs.w != 0, "Division by zero.");
	x = x / rhs.x;
	y = y / rhs.y;
	z = z / rhs.z;
	w = w / rhs.w;
	return *this;
}

Vector4& Vector4::add(const f32 rhs) {
	x = x + rhs;
	y = y + rhs;
	z = z + rhs;
	w = w + rhs;
	return *this;
}

Vector4& Vector4::sub(const f32 rhs) {
	x = x - rhs;
	y = y - rhs;
	z = z - rhs;
	w = w - rhs;
	return *this;
}

Vector4& Vector4::mul(const f32 rhs) {
	x = x * rhs;
	y = y * rhs;
	z = z * rhs;
	w = w * rhs;
	return *this;
}

Vector4& Vector4::div(const f32 rhs) {
	DE_ASSERT(rhs != 0, "Division by zero.");
	x = x / rhs;
	y = y / rhs;
	z = z / rhs;
	w = w / rhs;
	return *this;
}

f32 Vector4::dot(const Vector4& v) const {
	// SIMD-optimized
	f32 xx = x*v.x;
	f32 yy = y*v.y;
	f32 zz = z*v.z;
	f32 ww = w*v.w;

	f32 dot = xx;
	dot = dot+yy;
	dot = dot+zz;
	dot = dot+ww;

	return dot;
}

f32 Vector4::sqrlen() const {
	return this->dot(*this);
}

f32 Vector4::sqrdst(const Vector4& v) const {
	Vector4 sub = DE::Vector4(v)-(*this);
	return sub.dot(sub);
}

f32 Vector4::len() const {
	return sqrtf(this->sqrlen());
}

f32 Vector4::max() const {
	return std::max(x,std::max(y,std::max(z,w)));
}

f32 Vector4::min() const {
	return std::min(x,std::min(y,std::min(z,w)));
}

Vector4& Vector4::nor() {
	f32 len = this->len();

	DE_ASSERT(len > 0, "Length is zero.");
	this->div(len);

	return *this;
}

f32 Vector4::dst(const Vector4& v) const {
	return sqrtf(this->sqrdst(v));
}

bool Vector4::eq(const Vector4& v, const f32 e) const {
	return MathUtils::eqf(this->x, v.x, e) && MathUtils::eqf(this->y, v.y, e) &&
			MathUtils::eqf(this->z, v.z, e) && MathUtils::eqf(this->w, v.w, e);
}

bool Vector4::eq(const Vector4& v) const {
	return MathUtils::eqf(this->x, v.x) && MathUtils::eqf(this->y, v.y) &&
			MathUtils::eqf(this->z, v.z) && MathUtils::eqf(this->w, v.w);
}

Vector4& Vector4::lerp(const Vector4& target, const f32 t) {
	//start + percent*(end - start)
	(*this) += (Vector4(target)-(*this))*t;
	return *this;
}

Vector4& Vector4::clamp(const f32 maxLength) {
	if(this->sqrlen() > maxLength*maxLength){
		this->nor();
		this->mul(maxLength);
	}

	return *this;
}

} /* namespace DE */
