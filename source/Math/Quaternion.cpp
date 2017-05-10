#include "Quaternion.h"

#include <cmath>
#include <algorithm>    // std::max
#include "Assert.h"
#include "BasicTypes.h"

namespace DE {

Quaternion::Quaternion(f32 x, f32 y, f32 z, f32 w):v(x,y,z),w(w){
}

Quaternion::Quaternion(const Vector3& v, f32 w):v(v),w(w){
}

Quaternion::Quaternion():v(),w(1.0f){ // identity 0,0,0,1
}

Quaternion::Quaternion(f32 roll, f32 pitch, f32 yaw){

	// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

	f32 roll2 = rad(roll)*0.5f; // x
	f32 pitch2 = rad(pitch)*0.5f; // y
	f32 yaw2 = rad(yaw)*0.5f; // z

	f32 c3 = cosf(yaw2);
	f32 c2 = cosf(pitch2);
	f32 c1 = cosf(roll2);

	f32 s3 = sinf(yaw2);
	f32 s2 = sinf(pitch2);
	f32 s1 = sinf(roll2);

	// original
	// w   = c3*c2*c1 + s3*s2*s1;
	// v.x = c3*c2*s1 - s3*s2*c1;
	// v.y = c3*s2*c1 + s3*c2*s1;
	// v.z = s3*c2*c1 - c3*s2*s1;

	// sse-optimized
	f32 c3c2 = c3*c2;
	f32 s3s2 = s3*s2;
	f32 c3s2 = c3*s2;
	f32 s3c2 = s3*c2;

	f32 aux0 = c3c2*c1;
	f32 aux1 = c3c2*s1;
	f32 aux2 = c3s2*c1;
	f32 aux3 = s3c2*c1;

	f32 aux4 = s3s2*s1;
	f32 aux5 = s3s2*c1;
	f32 aux6 = s3c2*s1;
	f32 aux7 = c3s2*s1;

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


Quaternion& Quaternion::set(f32 x, f32 y, f32 z, f32 w) {
	if (this->w == w ) return *this; // handle self assignment
	//assignment operator
	v.set(x,y,z);
	this->w = w;
	return *this;
}

Quaternion& Quaternion::set(const Vector3& v, f32 w) {
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
	f32 w_total = (w * rhs.w) - (v.dot(rhs.v));
	v.set(rhs.v*w + v*w + Vector3(v).cross(rhs.v));
	w = w_total;
	return *this;
}

Quaternion& Quaternion::div(const Quaternion& rhs) {
	this->mul(Quaternion(rhs).inv());
	return *this;
}

Quaternion& Quaternion::add(const f32 rhs) {
	v.add(rhs);
	w = w + rhs;
	return *this;
}

Quaternion& Quaternion::sub(const f32 rhs) {
	v.sub(rhs);
	w = w - rhs;
	return *this;
}

Quaternion& Quaternion::mul(const f32 rhs) {
	v.mul(rhs);
	w = w * rhs;
	return *this;
}

Quaternion& Quaternion::div(const f32 rhs) {
	assert(rhs != 0, "Division by zero.");
	v.div(rhs);
	w = w / rhs;
	return *this;
}

f32 Quaternion::dot(const Quaternion& q) const {
	// sse-optimized
	f32 xx = v.x*q.v.x;
	f32 yy = v.y*q.v.y;
	f32 zz = v.z*q.v.z;
	f32 ww = w*q.w;

	return xx+yy+zz+ww;
}

f32 Quaternion::sqrlen() const {
	return dot(*this);
}

f32 Quaternion::len() const {
	return sqrtf(this->sqrlen());
}

Quaternion& Quaternion::nor() {
	f32 len = this->len();

	assert(len > 0, "Length is zero.");
	this->div(len);

	return *this;
}

bool Quaternion::eq(const Quaternion& q, f32 e) const {
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


f32 Quaternion::angle(const Quaternion& q) const{
	/*
	* angle is acute (positive dot product)
	* perpendicular (zero dot product)
	* or obtuse (negative dot product).
	*/
	// between 0 - 180
	return acosf(v.dot(q.v)/(v.len()*q.v.len()));
}

Quaternion& Quaternion::lerp(const Quaternion& target, f32 t) {

	f32 tt = 1-t;
	this->mul(tt);
	this->add(Quaternion(target).mul(tt));
	return *this;
}

Quaternion& Quaternion::nlerp(const Quaternion& target, f32 t) {
	this->lerp(target,t).nor();
	return *this;
}

Quaternion& Quaternion::slerp(const Quaternion& target, f32 t) {

	f32 theta = angle(target);

	f32 sinTheta = sinf(theta);

	this->mul((sinf(1-t)*theta)/sinTheta)
	.add(Quaternion(target).mul((sinf(t*theta))/sinTheta));
	return *this;
}

Vector3 Quaternion::toEuler() const{

	// https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles

	f32 xx = v.x * v.x;
	f32 yy = v.y * v.y;
	f32 zz = v.z * v.z;
	f32 zx = v.z * v.x;
	f32 yz = v.y * v.z;
	f32 xy = v.x * v.y;
	f32 wx = w * v.x;
	f32 wy = w * v.y;
	f32 wz = w * v.z;

	// roll (x-axis rotation)
	f32 t0 = +2.0 * (wx + yz);
	f32 t1 = +1.0 - 2.0 * (xx + yy);
	f32 roll = atan2f(t0, t1);

	// pitch (y-axis rotation)
	f32 t2 = +2.0 * (wy * v.y - zx);
	t2 = t2 > 1.0 ? 1.0 : t2;
	t2 = t2 < -1.0 ? -1.0 : t2;
	f32 pitch = asinf(t2);

	// yaw (z-axis rotation)
	f32 t3 = +2.0 * (wz * v.z + xy);
	f32 t4 = +1.0 - 2.0 * (yy + zz);
	f32 yaw = atan2f(t3, t4);

	return Vector3(deg(roll),deg(pitch),deg(yaw));
}

} /* namespace DE */
