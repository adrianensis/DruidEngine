#ifndef QUATERNION_H_
#define QUATERNION_H_

#include <ostream>
#include <cassert>
#include "Aligned16.h"
#include "MathUtils.h"
#include "Vector3.h"

namespace DE {

class Quaternion : public Aligned16 {
public:

	//-------------------------------------------------------------------
	// MEMBERS
	//-------------------------------------------------------------------

	float w;
	Vector3 v;

	//-------------------------------------------------------------------
	// CONSTRUCTORS/DESTRUCTOR
	//-------------------------------------------------------------------

	Quaternion();
	Quaternion(float x, float y, float z, float w);
	Quaternion(const Vector3& v, float w);
	Quaternion(float theta_x, float theta_y, float theta_z);
	Quaternion(const Vector3& v);
	Quaternion(const Quaternion& other);
	~Quaternion();

	//-------------------------------------------------------------------
	// METHODS
	//-------------------------------------------------------------------

	Quaternion& set(float x, float y, float z, float w);
	Quaternion& set(const Vector3& v, float w);
	Quaternion& set(const Quaternion& rhs);
	Quaternion& add(const Quaternion& rhs);
	Quaternion& sub(const Quaternion& rhs);
	Quaternion& mul(const Quaternion& rhs);
	Quaternion& div(const Quaternion& rhs);
	Quaternion& add(const float rhs);
	Quaternion& sub(const float rhs);
	Quaternion& mul(const float rhs);
	Quaternion& div(const float rhs);

	float len() const;
	float sqrlen() const;
	float dot(const Quaternion& q) const;
	Quaternion& nor();
	bool eq(const Quaternion& q, float e) const; // epsilon equal
	bool eq(const Quaternion& q) const; // equal
	Quaternion& conj();
	Quaternion& inv();
	Quaternion& lerp(const Quaternion& target, float t);
	Quaternion& nlerp(const Quaternion& target, float t);
	Quaternion& slerp(const Quaternion& target, float t);
	Quaternion& squad(); // TODO: implement
	Vector3 toEuler() const; // TODO: implement
	float angle(const Quaternion& q) const;

	//-------------------------------------------------------------------
	// OPERATORS
	//-------------------------------------------------------------------

	/*
	 * & -> it's a reference, not a full copy.
	 * const Quaternion& -> it's a constant reference.
	 * operator=(...) const -> the method promises not to change *this. Non-member
	 * functions can not have constant qualification.
	 */

	Quaternion& operator=(const Quaternion& rhs) {
		// if (this == &rhs) return *this; // handle self assignment
		//assignment operator
		return this->set(rhs);
	}

	Quaternion& operator+=(const Quaternion& rhs) {
		// can be parallelized with sse auto-vectorization
		return this->add(rhs);
	}

	Quaternion& operator-=(const Quaternion& rhs) {
		return this->sub(rhs);
	}

	Quaternion& operator*=(const Quaternion& rhs) {
		return this->mul(rhs);
	}

	Quaternion& operator/=(const Quaternion& rhs) {
		return this->div(rhs);
	}

	Quaternion& operator+=(const float rhs) {
		return this->add(rhs);
	}

	Quaternion& operator-=(const float rhs) {
		return this->sub(rhs);
	}

	Quaternion& operator*=(const float rhs) {
		return this->mul(rhs);
	}

	Quaternion& operator/=(const float rhs) {
		return this->div(rhs);
	}

	bool operator==(const Quaternion& rhs) const {
		return this->eq(rhs);
	}

	bool operator!=(const Quaternion& rhs) const {
		return !((*this) == rhs);
	}

	Quaternion operator+(const Quaternion& rhs) const {
		return Quaternion(*this) += rhs;
	}

	Quaternion operator-(const Quaternion& rhs) const {
		return Quaternion(*this) -= rhs;
	}

	Quaternion operator*(const Quaternion& rhs) const {
		return Quaternion(*this) *= rhs;
	}

	Quaternion operator/(const Quaternion& rhs) const {
		return Quaternion(*this) /= rhs;
	}

	Quaternion operator+(const float rhs) const {
		return Quaternion(*this) += rhs;
	}

	Quaternion operator-(const float rhs) const {
		return Quaternion(*this) -= rhs;
	}

	Quaternion operator*(const float rhs) const {
		return Quaternion(*this) *= rhs;
	}

	Quaternion operator/(const float rhs) const {
		return Quaternion(*this) /= rhs;
	}

	// can be used for assignment
	float& operator[](const size_t i) {
		assert(i >= 0 && i < 4);
		if(i == 3) return w;
		return v[i];
	}

	// read only
	float operator[](const size_t i) const {
		assert(i >= 0 && i < 4);
		if(i == 3) return w;
		return v[i];
	}

	friend std::ostream& operator<<(std::ostream& out, const Quaternion& q) {
		out << "[(" << q.v.x << ", " << q.v.y << ", " << q.v.z << "), " << q.w << "]";
		return out;
	}
};

} /* namespace DE */

#endif /* QUATERNION_H_ */
