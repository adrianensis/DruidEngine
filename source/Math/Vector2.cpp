#include "Vector2.h"

#include <cmath>
#include <algorithm>    // std::max
#include <cassert>

namespace DE {

Vector2::Vector2(float x, float y):x(x),y(y){
}

Vector2::Vector2():x(0.0f),y(0.0f){
}

Vector2::~Vector2(){
}

Vector2::Vector2(const Vector2& other):x(other.x),y(other.y){
}

Vector2& Vector2::set(float x, float y) {
	if (this->x == x && this->y == y) return *this; // handle self assignment
	//assignment operator
	this->x = x;
	this->y = y;
	return *this;
}

Vector2& Vector2::set(const Vector2& rhs) {
	if (this == &rhs) return *this; // handle self assignment
	//assignment operator
	this->set(rhs.x,rhs.y);
	return *this;
}

Vector2& Vector2::add(const Vector2& rhs) {
	// can be parallelized with sse auto-vectorization
	x = x + rhs.x;
	y = y + rhs.y;
	return *this;
}

Vector2& Vector2::sub(const Vector2& rhs) {
	x = x - rhs.x;
	y = y - rhs.y;
	return *this;
}

Vector2& Vector2::mul(const Vector2& rhs) {
	x = x * rhs.x;
	y = y * rhs.y;
	return *this;
}

Vector2& Vector2::div(const Vector2& rhs) {
	assert(rhs.x != 0);
	assert(rhs.y != 0);
	x = x / rhs.x;
	y = y / rhs.y;
	return *this;
}

Vector2& Vector2::add(const float rhs) {
	x = x + rhs;
	y = y + rhs;
	return *this;
}

Vector2& Vector2::sub(const float rhs) {
	x = x - rhs;
	y = y - rhs;
	return *this;
}

Vector2& Vector2::mul(const float rhs) {
	x = x * rhs;
	y = y * rhs;
	return *this;
}

Vector2& Vector2::div(const float rhs) {
	assert(rhs != 0);
	x = x / rhs;
	y = y / rhs;
	return *this;
}

float Vector2::dot(const Vector2& v) const {
	return this->x*v.x + this->y*v.y;
}

float Vector2::sqrlen() const {
	return this->dot(*this);
}

float Vector2::sqrdst(const Vector2& v) const {
	Vector2 sub = DE::Vector2(v)-(*this);
	return sub.dot(sub);
}

float Vector2::len() const {
	return sqrtf(this->sqrlen());
}

float Vector2::max() const {
	return std::max(x,y);
}

float Vector2::min() const {
	return std::min(x,y);
}

Vector2& Vector2::nor() {
	float len = this->len();

	assert(len > 0);
	this->div(len);

	return *this;
}

float Vector2::dst(const Vector2& v) const {
	return sqrtf(this->sqrdst(v));
}

bool Vector2::eq(const Vector2& v) const {
	return eqf(this->x, v.x) && eqf(this->y, v.y);
}

bool Vector2::eq(const Vector2& v, float e) const {
	return eqf(this->x, v.x, e) && eqf(this->y, v.y, e);
}

Vector2& Vector2::lerp(const Vector2& target, float t) {
	//start + percent*(end - start)
	(*this) += (Vector2(target)-(*this))*t;
	return *this;
}

float Vector2::angle(const Vector2& v) const {
	float angle = atan2f(v.y, v.x) - atan2f(this->y, this->x);
	return angle < 0 ? angle += 2 * M_PI : angle;
}

Vector2& Vector2::clamp(float maxLength) {
	if(this->sqrlen() > maxLength*maxLength){
		this->nor();
		this->mul(maxLength);
	}

	return *this;
}


} /* namespace DE */
