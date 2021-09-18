#pragma once

#include "Core/Core.hpp"


#include "Core/BasicTypes.hpp"
#include "Core/Assert/Assert.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Matrix4.hpp"

CLASS(Quaternion, ObjectBase) /*16 bytes alignment*/
{
public:
	//-------------------------------------------------------------------
	// MEMBERS
	//-------------------------------------------------------------------

	f32 w;
	Vector3 v;

	//-------------------------------------------------------------------
	// CONSTRUCTORS/DESTRUCTOR
	//-------------------------------------------------------------------

	Quaternion();
	Quaternion(f32 x, f32 y, f32 z, f32 w);
	Quaternion(CNS Vector3 &v, f32 w);
	Quaternion(f32 roll, f32 pitch, f32 yaw);
	Quaternion(CNS Vector3 &v);
	Quaternion(CNS Quaternion &other);

	//-------------------------------------------------------------------
	// METHODS
	//-------------------------------------------------------------------

	Quaternion &set(f32 x, f32 y, f32 z, f32 w);
	Quaternion &set(CNS Vector3 &v, f32 w);
	Quaternion &set(CNS Quaternion &rhs);
	Quaternion &add(CNS Quaternion &rhs);
	Quaternion &sub(CNS Quaternion &rhs);
	Quaternion &mul(CNS Quaternion &rhs);
	Quaternion &div(CNS Quaternion &rhs);
	Quaternion &add(f32 rhs);
	Quaternion &sub(f32 rhs);
	Quaternion &mul(f32 rhs);
	Quaternion &div(f32 rhs);

	f32 len() CNS;
	f32 sqrlen() CNS;
	f32 dot(CNS Quaternion &q) CNS;
	Quaternion &nor();
	bool eq(CNS Quaternion &q, f32 e) CNS; // epsilon equal
	bool eq(CNS Quaternion &q) CNS;		   // equal
	Quaternion &conj();
	Quaternion &inv();
	f32 angle(CNS Quaternion &q) CNS;
	Quaternion &lerp(CNS Quaternion &target, f32 t);
	Quaternion &nlerp(CNS Quaternion &target, f32 t);
	Quaternion &slerp(CNS Quaternion &target, f32 t);
	Quaternion &squad(); // TODO: implement
	void fromEuler(f32 roll, f32 pitch, f32 yaw);
	void fromEuler(CNS Vector3 &v);
	//Vector3 toEuler() CNS;
	void toMatrix(Matrix4 * outMatrix) CNS;
	//void fromMatrix(CNS Matrix4 &m);

	//-------------------------------------------------------------------
	// OPERATORS
	//-------------------------------------------------------------------

	/*
	* & -> it's a reference, not a full copy.
	* Quaternion& -> it's a constant reference.
	* operator=(...) CNS -> the method promises not to change *this. Non-member
	* functions can not have constant qualification.
	*/

	Quaternion &operator=(CNS Quaternion &rhs)
	{
			return this->set(rhs);
	}

	Quaternion &operator+=(CNS Quaternion &rhs)
	{
		// can be parallelized with SIMD auto-vectorization
		return this->add(rhs);
	}

	Quaternion &operator-=(CNS Quaternion &rhs)
	{
		return this->sub(rhs);
	}

	Quaternion &operator*=(CNS Quaternion &rhs)
	{
		return this->mul(rhs);
	}

	Quaternion &operator/=(CNS Quaternion &rhs)
	{
		return this->div(rhs);
	}

	Quaternion &operator+=(f32 rhs)
	{
		return this->add(rhs);
	}

	Quaternion &operator-=(f32 rhs)
	{
		return this->sub(rhs);
	}

	Quaternion &operator*=(f32 rhs)
	{
		return this->mul(rhs);
	}

	Quaternion &operator/=(f32 rhs)
	{
		return this->div(rhs);
	}

	bool operator==(CNS Quaternion &rhs) CNS
	{
		return this->eq(rhs);
	}

	bool operator!=(CNS Quaternion &rhs) CNS
	{
		return !((*this) == rhs);
	}

	Quaternion operator+(CNS Quaternion &rhs) CNS
	{
		return Quaternion(*this) += rhs;
	}

	Quaternion operator-(CNS Quaternion &rhs) CNS
	{
		return Quaternion(*this) -= rhs;
	}

	Quaternion operator*(CNS Quaternion &rhs) CNS
	{
		return Quaternion(*this) *= rhs;
	}

	Quaternion operator/(CNS Quaternion &rhs) CNS
	{
		return Quaternion(*this) /= rhs;
	}

	Quaternion operator+(f32 rhs) CNS
	{
		return Quaternion(*this) += rhs;
	}

	Quaternion operator-(f32 rhs) CNS
	{
		return Quaternion(*this) -= rhs;
	}

	Quaternion operator*(f32 rhs) CNS
	{
		return Quaternion(*this) *= rhs;
	}

	Quaternion operator/(f32 rhs) CNS
	{
		return Quaternion(*this) /= rhs;
	}

	// can be used for assignment
	f32 &operator[](size_t index)
	{
		ASSERT_MSG(index >= 0 && index < 4, "Index out of bounds.");
		if (index == 3)
			return w;
		return v[index];
	}

	// read only
	f32 operator[](size_t index) CNS
	{
		ASSERT_MSG(index >= 0 && index < 4, "Index out of bounds.");
		if (index == 3)
			return w;
		return v[index];
	}
};
