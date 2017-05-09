#ifndef VECTOR4_H_
#define VECTOR4_H_

#include <ostream>
#include "Assert.h"
#include "Aligned16.h"
#include "MathUtils.h"

namespace DE {

class Vector4 : public Aligned16 {
public:

	//-------------------------------------------------------------------
	// MEMBERS
	//-------------------------------------------------------------------

	float x, y, z, w;

	//-------------------------------------------------------------------
	// CONSTRUCTORS/DESTRUCTOR
	//-------------------------------------------------------------------

	Vector4();
	Vector4(float x, float y, float z, float w);
	Vector4(const Vector4& other);
	~Vector4();

	//-------------------------------------------------------------------
	// METHODS
	//-------------------------------------------------------------------

	virtual Vector4& set(const Vector4& rhs);
	Vector4& set(float x, float y, float z, float w);
	virtual Vector4& add(const Vector4& rhs);
	virtual Vector4& sub(const Vector4& rhs);
	virtual Vector4& mul(const Vector4& rhs);
	virtual Vector4& div(const Vector4& rhs);
	virtual Vector4& add(const float rhs);
	virtual Vector4& sub(const float rhs);
	virtual Vector4& mul(const float rhs);
	virtual Vector4& div(const float rhs);

	float len() const;
	float sqrlen() const;
	float max() const;
	float min() const;
	float dot(const Vector4& v) const;
	Vector4& nor();
	float dst(const Vector4& v) const;
	float sqrdst(const Vector4& v) const;
	bool eq(const Vector4& v, float e) const; // epsilon equal
	bool eq(const Vector4& v) const; // equal
	Vector4& lerp(const Vector4& target, float t);
	Vector4& clamp(float maxLength);

	//-------------------------------------------------------------------
	// OPERATORS
	//-------------------------------------------------------------------

	/*
	 * & -> it's a reference, not a full copy.
	 * const Vector4& -> it's a constant reference.
	 * operator=(...) const -> the method promises not to change *this. Non-member
	 * functions can not have constant qualification.
	 */

	Vector4& operator=(const Vector4& rhs) {
		// if (this == &rhs) return *this; // handle self assignment
		//assignment operator
		return this->set(rhs);
	}

	Vector4& operator+=(const Vector4& rhs) {
		// can be parallelized with sse auto-vectorization
		return this->add(rhs);
	}

	Vector4& operator-=(const Vector4& rhs) {
		return this->sub(rhs);
	}

	Vector4& operator*=(const Vector4& rhs) {
		return this->mul(rhs);
	}

	Vector4& operator/=(const Vector4& rhs) {
		return this->div(rhs);
	}

	Vector4& operator+=(const float rhs) {
		return this->add(rhs);
	}

	Vector4& operator-=(const float rhs) {
		return this->sub(rhs);
	}

	Vector4& operator*=(const float rhs) {
		return this->mul(rhs);
	}

	Vector4& operator/=(const float rhs) {
		return this->div(rhs);
	}

	bool operator==(const Vector4& rhs) const {
		return this->eq(rhs);
	}

	bool operator!=(const Vector4& rhs) const {
		return !((*this) == rhs);
	}

	Vector4 operator+(const Vector4& rhs) const {
		return Vector4(*this) += rhs;
	}

	Vector4 operator-(const Vector4& rhs) const {
		return Vector4(*this) -= rhs;
	}

	Vector4 operator*(const Vector4& rhs) const {
		return Vector4(*this) *= rhs;
	}

	Vector4 operator/(const Vector4& rhs) const {
		return Vector4(*this) /= rhs;
	}

	Vector4 operator+(const float rhs) const {
		return Vector4(*this) += rhs;
	}

	Vector4 operator-(const float rhs) const {
		return Vector4(*this) -= rhs;
	}

	Vector4 operator-() const {
		return Vector4(*this) *= -1;
	}

	Vector4 operator*(const float rhs) const {
		return Vector4(*this) *= rhs;
	}

	Vector4 operator/(const float rhs) const {
		return Vector4(*this) /= rhs;
	}

	// can be used for assignment
	float& operator[](const size_t i) {
		assert(i >= 0 && i < 4, "Index out of bounds.");

		return *(&x+i);
	}

	// read only
	float operator[](const size_t i) const {
		assert(i >= 0 && i < 4, "Index out of bounds.");
		return *(&x+i);
	}

	friend std::ostream& operator<<(std::ostream& out, const Vector4& v) {
		out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		return out;
	}
};

} /* namespace DE */

#endif /* VECTOR4_H_ */
