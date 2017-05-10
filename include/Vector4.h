#ifndef VECTOR4_H_
#define VECTOR4_H_

#include <ostream>
#include "Assert.h"
#include "Aligned16.h"
#include "BasicTypes.h"
#include "MathUtils.h"

namespace DE {

class Vector4 /*: public Aligned16*/ {
public:

	//-------------------------------------------------------------------
	// MEMBERS
	//-------------------------------------------------------------------

	f32 x, y, z, w;

	//-------------------------------------------------------------------
	// CONSTRUCTORS/DESTRUCTOR
	//-------------------------------------------------------------------

	Vector4();
	Vector4(f32 x, f32 y, f32 z, f32 w);
	Vector4(const Vector4& other);
	~Vector4();

	//-------------------------------------------------------------------
	// METHODS
	//-------------------------------------------------------------------

	virtual Vector4& set(const Vector4& rhs);
	Vector4& set(f32 x, f32 y, f32 z, f32 w);
	virtual Vector4& add(const Vector4& rhs);
	virtual Vector4& sub(const Vector4& rhs);
	virtual Vector4& mul(const Vector4& rhs);
	virtual Vector4& div(const Vector4& rhs);
	virtual Vector4& add(const f32 rhs);
	virtual Vector4& sub(const f32 rhs);
	virtual Vector4& mul(const f32 rhs);
	virtual Vector4& div(const f32 rhs);

	f32 len() const;
	f32 sqrlen() const;
	f32 max() const;
	f32 min() const;
	f32 dot(const Vector4& v) const;
	Vector4& nor();
	f32 dst(const Vector4& v) const;
	f32 sqrdst(const Vector4& v) const;
	bool eq(const Vector4& v, f32 e) const; // epsilon equal
	bool eq(const Vector4& v) const; // equal
	Vector4& lerp(const Vector4& target, f32 t);
	Vector4& clamp(f32 maxLength);

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

	Vector4& operator+=(const f32 rhs) {
		return this->add(rhs);
	}

	Vector4& operator-=(const f32 rhs) {
		return this->sub(rhs);
	}

	Vector4& operator*=(const f32 rhs) {
		return this->mul(rhs);
	}

	Vector4& operator/=(const f32 rhs) {
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

	Vector4 operator+(const f32 rhs) const {
		return Vector4(*this) += rhs;
	}

	Vector4 operator-(const f32 rhs) const {
		return Vector4(*this) -= rhs;
	}

	Vector4 operator-() const {
		return Vector4(*this) *= -1;
	}

	Vector4 operator*(const f32 rhs) const {
		return Vector4(*this) *= rhs;
	}

	Vector4 operator/(const f32 rhs) const {
		return Vector4(*this) /= rhs;
	}

	// can be used for assignment
	f32& operator[](const size_t i) {
		assert(i >= 0 && i < 4, "Index out of bounds.");

		return *(&x+i);
	}

	// read only
	f32 operator[](const size_t i) const {
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
