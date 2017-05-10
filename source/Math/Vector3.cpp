#include "Vector3.h"

#include <cmath>
#include <algorithm>    // std::max
#include "Assert.h"
#include "MathUtils.h"
#include "BasicTypes.h"

namespace DE {

Vector3::Vector3(f32 x, f32 y, f32 z):x(x),y(y),z(z){
}

Vector3::Vector3():x(0.0f),y(0.0f),z(0.0f){
}

Vector3::~Vector3(){
}

Vector3::Vector3(const Vector3& other):x(other.x),y(other.y),z(other.z){
}

Vector3& Vector3::set(f32 x, f32 y, f32 z) {
	if (this->x == x && this->y == y && this->z == z) return *this; // handle self assignment
	//assignment operator
	this->x = x;
	this->y = y;
	this->z = z;
	return *this;
}

Vector3& Vector3::set(const Vector3& rhs) {
	if (this == &rhs) return *this; // handle self assignment
	//assignment operator
	this->set(rhs.x,rhs.y,rhs.z);
	return *this;
}

Vector3& Vector3::add(const Vector3& rhs) {
	// can be parallelized with sse auto-vectorization
	x = x + rhs.x;
	y = y + rhs.y;
	z = z + rhs.z;
	return *this;
}

Vector3& Vector3::sub(const Vector3& rhs) {
	x = x - rhs.x;
	y = y - rhs.y;
	z = z - rhs.z;
	return *this;
}

Vector3& Vector3::mul(const Vector3& rhs) {
	x = x * rhs.x;
	y = y * rhs.y;
	z = z * rhs.z;
	return *this;
}

Vector3& Vector3::div(const Vector3& rhs) {
	assert(rhs.x != 0, "Division by zero.");
	assert(rhs.y != 0, "Division by zero.");
	assert(rhs.z != 0, "Division by zero.");
	x = x / rhs.x;
	y = y / rhs.y;
	z = z / rhs.z;
	return *this;
}

Vector3& Vector3::add(const f32 rhs) {
	x = x + rhs;
	y = y + rhs;
	z = z + rhs;
	return *this;
}

Vector3& Vector3::sub(const f32 rhs) {
	x = x - rhs;
	y = y - rhs;
	z = z - rhs;
	return *this;
}

Vector3& Vector3::mul(const f32 rhs) {
	x = x * rhs;
	y = y * rhs;
	z = z * rhs;
	return *this;
}

Vector3& Vector3::div(const f32 rhs) {
	assert(rhs != 0, "Division by zero.");
	x = x / rhs;
	y = y / rhs;
	z = z / rhs;
	return *this;
}

f32 Vector3::dot(const Vector3& v) const {
	return this->x*v.x + this->y*v.y + this->z*v.z;
}

f32 Vector3::sqrlen() const {
	return this->dot(*this);
}

f32 Vector3::sqrdst(const Vector3& v) const {
	Vector3 sub = DE::Vector3(v)-(*this);
	return sub.dot(sub);
}

f32 Vector3::len() const {
	return sqrtf(this->sqrlen());
}

f32 Vector3::max() const {
	return std::max(x,std::max(y,z));
}

f32 Vector3::min() const {
	return std::min(x,std::min(y,z));
}

Vector3& Vector3::nor() {
	f32 len = this->len();

	assert(len > 0, "Length is zero.");
	this->div(len);

	return *this;
}

f32 Vector3::dst(const Vector3& v) const {
	return sqrtf(this->sqrdst(v));
}

bool Vector3::eq(const Vector3& v) const {
	return eqf(this->x, v.x) && eqf(this->y, v.y) && eqf(this->z, v.z);
}

bool Vector3::eq(const Vector3& v, f32 e) const {
	return eqf(this->x, v.x, e) && eqf(this->y, v.y, e) && eqf(this->z, v.z, e);
}

Vector3& Vector3::cross(const Vector3& v){
	f32 x1 = this->x;
  f32 y1 = this->y;
  f32 z1 = this->z;

  f32 x2 = v.x;
  f32 y2 = v.y;
  f32 z2 = v.z;

  this->x = y1 * z2 - z1 * y2;
	this->y = z1 * x2 - x1 * z2;
	this->z = x1 * y2 - y1 * x2;

	return *this;
}

Vector3& Vector3::lerp(const Vector3& target, f32 t) {
	//start + percent*(end - start)
	(*this) += (Vector3(target)-(*this))*t;
	return *this;
}

Vector3& Vector3::nlerp(const Vector3& target, f32 t) {
	//start + percent*(end - start)
	this->lerp(target,t).nor();
	return *this;
}

Vector3& Vector3::slerp(const Vector3& target, f32 t) {

	if (t == 0) return *this;

	f32 theta = angle(target);
	//f32 thetaDegree = theta * RadianToDegree;

	f32 sinTheta = sinf(theta);

	return this->mul(sinf((1-t)*theta)/sinTheta)
	.add(target*(sinf(t*theta)/sinTheta));
}

f32 Vector3::angle(const Vector3& v) const {
	/*
	* angle is acute (positive dot product)
	* perpendicular (zero dot product)
	* or obtuse (negative dot product).
	*/
	// between 0 - 180
	return acosf(this->dot(v)/(this->len()*v.len()));
}

f32 Vector3::angle(const Vector3& v, const Vector3& n) const {

	f32 dot = this->dot(v);
	Vector3 cross = Vector3(*this).cross(v);
	f32 angle = atan2f(cross.len(),dot);

	angle = n.dot(cross) < 0.0f ? (2.0f*M_PI)-angle : angle;

	return angle;
}

Vector3& Vector3::clamp(f32 maxLength) {
	if(this->sqrlen() > maxLength*maxLength){
		this->nor();
		this->mul(maxLength);
	}

	return *this;
}

} /* namespace DE */
