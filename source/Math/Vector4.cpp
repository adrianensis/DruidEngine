#include "Vector4.h"

#include <cmath>
#include <algorithm>    // std::max
#include <cassert>
#include "MathUtils.h"

namespace DE {

Vector4::Vector4(float x, float y, float z, float w):x(x),y(y),z(z),w(w){
}

Vector4::Vector4():x(0.0f),y(0.0f),z(0.0f),w(0.0f){
}

Vector4::~Vector4(){
}

Vector4::Vector4(const Vector4& other):x(other.x),y(other.y),z(other.z),w(other.w){
}

Vector4& Vector4::set(float x, float y, float z, float w) {
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
	// can be parallelized with sse auto-vectorization
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
	assert(rhs.x != 0);
	assert(rhs.y != 0);
	assert(rhs.z != 0);
	assert(rhs.w != 0);
	x = x / rhs.x;
	y = y / rhs.y;
	z = z / rhs.z;
	w = w / rhs.w;
	return *this;
}

Vector4& Vector4::add(const float rhs) {
	x = x + rhs;
	y = y + rhs;
	z = z + rhs;
	w = w + rhs;
	return *this;
}

Vector4& Vector4::sub(const float rhs) {
	x = x - rhs;
	y = y - rhs;
	z = z - rhs;
	w = w - rhs;
	return *this;
}

Vector4& Vector4::mul(const float rhs) {
	x = x * rhs;
	y = y * rhs;
	z = z * rhs;
	w = w * rhs;
	return *this;
}

Vector4& Vector4::div(const float rhs) {
	assert(rhs != 0);
	x = x / rhs;
	y = y / rhs;
	z = z / rhs;
	w = w / rhs;
	return *this;
}

float Vector4::dot(const Vector4& v) const {
	// sse-optimized
	float xx = x*v.x;
	float yy = y*v.y;
	float zz = z*v.z;
	float ww = w*v.w;

	return xx+yy+zz+ww;
}

float Vector4::sqrlen() const {
	return this->dot(*this);
}

float Vector4::sqrdst(const Vector4& v) const {
	Vector4 sub = DE::Vector4(v)-(*this);
	return sub.dot(sub);
}

float Vector4::len() const {
	return sqrtf(this->sqrlen());
}

float Vector4::max() const {
	return std::max(x,std::max(y,std::max(z,w)));
}

float Vector4::min() const {
	return std::min(x,std::min(y,std::min(z,w)));
}

Vector4& Vector4::nor() {
	float len = this->len();

	assert(len > 0);
	this->div(len);

	return *this;
}

float Vector4::dst(const Vector4& v) const {
	return sqrtf(this->sqrdst(v));
}

bool Vector4::eq(const Vector4& v, float e) const {
	return eqf(this->x, v.x, e) && eqf(this->y, v.y, e) &&
	eqf(this->z, v.z, e) && eqf(this->w, v.w, e);
}

bool Vector4::eq(const Vector4& v) const {
	return eqf(this->x, v.x) && eqf(this->y, v.y) &&
	eqf(this->z, v.z) && eqf(this->w, v.w);
}

Vector4& Vector4::lerp(const Vector4& target, float t) {
	//start + percent*(end - start)
	(*this) += (Vector4(target)-(*this))*t;
	return *this;
}

Vector4& Vector4::clamp(float maxLength) {
	if(this->sqrlen() > maxLength*maxLength){
		this->nor();
		this->mul(maxLength);
	}

	return *this;
}

} /* namespace DE */
