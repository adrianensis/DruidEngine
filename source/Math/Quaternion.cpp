#include "Quaternion.h"

#include <cmath>
#include <algorithm>    // std::max
#include <cassert>

namespace DE {

Quaternion::Quaternion(float x, float y, float z, float w):v(x,y,z),w(w){
}

Quaternion::Quaternion(const Vector3& v, float w):v(v),w(w){
}

Quaternion::Quaternion():v(),w(1.0f){ // identity 0,0,0,1
}

Quaternion::Quaternion(float roll, float pitch, float yaw){

	// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

	float roll2 = rad(roll)*0.5f; // x
	float pitch2 = rad(pitch)*0.5f; // y
	float yaw2 = rad(yaw)*0.5f; // z

	float c3 = cosf(yaw2);
	float c2 = cosf(pitch2);
	float c1 = cosf(roll2);

	float s3 = sinf(yaw2);
	float s2 = sinf(pitch2);
	float s1 = sinf(roll2);

	// original
	// w   = c3*c2*c1 + s3*s2*s1;
	// v.x = c3*c2*s1 - s3*s2*c1;
	// v.y = c3*s2*c1 + s3*c2*s1;
	// v.z = s3*c2*c1 - c3*s2*s1;

	// sse-optimized
	float c3c2 = c3*c2;
	float s3s2 = s3*s2;
	float c3s2 = c3*s2;
	float s3c2 = s3*c2;

	float aux0 = c3c2*c1;
	float aux1 = c3c2*s1;
	float aux2 = c3s2*c1;
	float aux3 = s3c2*c1;

	float aux4 = s3s2*s1;
	float aux5 = s3s2*c1;
	float aux6 = s3c2*s1;
	float aux7 = c3s2*s1;

	w   = aux0 + aux4;
	v.x = aux1 - aux5;
	v.y = aux2 + aux6;
	v.z = aux3 - aux7;

}

Quaternion::Quaternion(const Vector3& v):Quaternion(v.x,v.y,v.z){
}

Quaternion::Quaternion(const Quaternion& other):v(other.v),w(other.w){
}

Quaternion::~Quaternion(){
}


Quaternion& Quaternion::set(float x, float y, float z, float w) {
	if (this->w == w ) return *this; // handle self assignment
	//assignment operator
	v.set(x,y,z);
	this->w = w;
	return *this;
}

Quaternion& Quaternion::set(const Vector3& v, float w) {
	if (this->w == w ) return *this; // handle self assignment
	//assignment operator
	this->v.set(v);
	this->w = w;
	return *this;
}

Quaternion& Quaternion::set(const Quaternion& rhs) {
	if (this == &rhs) return *this; // handle self assignment
	//assignment operator
	set(rhs.v, rhs.w);
	return *this;
}

Quaternion& Quaternion::add(const Quaternion& rhs) {
	// can be parallelized with sse auto-vectorization
	v.add(rhs.v);
	w = w + rhs.w;
	return *this;
}

Quaternion& Quaternion::sub(const Quaternion& rhs) {
	v.sub(rhs.v);
	w = w - rhs.w;
	return *this;
}

Quaternion& Quaternion::mul(const Quaternion& rhs) {
	float w_total = (w * rhs.w) - (v.dot(rhs.v));
	v.set(rhs.v*w + v*w + Vector3(v).cross(rhs.v));
	w = w_total;
	return *this;
}

Quaternion& Quaternion::div(const Quaternion& rhs) {
	this->mul(Quaternion(rhs).inv());
	return *this;
}

Quaternion& Quaternion::add(const float rhs) {
	v.add(rhs);
	w = w + rhs;
	return *this;
}

Quaternion& Quaternion::sub(const float rhs) {
	v.sub(rhs);
	w = w - rhs;
	return *this;
}

Quaternion& Quaternion::mul(const float rhs) {
	v.mul(rhs);
	w = w * rhs;
	return *this;
}

Quaternion& Quaternion::div(const float rhs) {
	assert(rhs != 0);
	v.div(rhs);
	w = w / rhs;
	return *this;
}

float Quaternion::dot(const Quaternion& q) const {
	// sse-optimized
	float xx = v.x*q.v.x;
	float yy = v.y*q.v.y;
	float zz = v.z*q.v.z;
	float ww = w*q.w;

	return xx+yy+zz+ww;
}

float Quaternion::sqrlen() const {
	return dot(*this);
}

float Quaternion::len() const {
	return sqrtf(this->sqrlen());
}

Quaternion& Quaternion::nor() {
	float len = this->len();

	assert(len > 0);
	this->div(len);

	return *this;
}

bool Quaternion::eq(const Quaternion& q, float e) const {
	return v.eq(q.v, e) && eqf(this->w, q.w, e);
}

bool Quaternion::eq(const Quaternion& q) const {
	return v.eq(q.v) && eqf(this->w, q.w);
}

Quaternion& Quaternion::conj() {
	this->v.mul(-1);
	return *this;
}

Quaternion& Quaternion::inv() {
	this->conj().div(this->sqrlen());
	return *this;
}


float Quaternion::angle(const Quaternion& q) const{
	/*
	* angle is acute (positive dot product)
	* perpendicular (zero dot product)
	* or obtuse (negative dot product).
	*/
	// between 0 - 180
	return acosf(v.dot(q.v)/(v.len()*q.v.len()));
}

Quaternion& Quaternion::lerp(const Quaternion& target, float t) {

	float tt = 1-t;
	this->mul(tt);
	this->add(Quaternion(target).mul(tt));
	return *this;
}

Quaternion& Quaternion::nlerp(const Quaternion& target, float t) {
	this->lerp(target,t).nor();
	return *this;
}

Quaternion& Quaternion::slerp(const Quaternion& target, float t) {

	float theta = angle(target);

	float sinTheta = sinf(theta);

	this->mul((sinf(1-t)*theta)/sinTheta)
	.add(Quaternion(target).mul((sinf(t*theta))/sinTheta));
	return *this;
}

Vector3 Quaternion::toEuler() const{

	// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

	float xx = v.x * v.x;
	float yy = v.y * v.y;
	float zz = v.z * v.z;
	float zx = v.z * v.x;
	float yz = v.y * v.z;
	float xy = v.x * v.y;
	float wx = w * v.x;
	float wy = w * v.y;
	float wz = w * v.z;

	// roll (x-axis rotation)
	float t0 = +2.0 * (wx + yz);
	float t1 = +1.0 - 2.0 * (xx + yy);
	float roll = atan2f(t0, t1);

	// pitch (y-axis rotation)
	float t2 = +2.0 * (wy * v.y - zx);
	t2 = t2 > 1.0 ? 1.0 : t2;
	t2 = t2 < -1.0 ? -1.0 : t2;
	float pitch = asinf(t2);

	// yaw (z-axis rotation)
	float t3 = +2.0 * (wz * v.z + xy);
	float t4 = +1.0 - 2.0 * (yy + zz);
	float yaw = atan2f(t3, t4);

	return Vector3(deg(roll),deg(pitch),deg(yaw));
}

} /* namespace DE */
