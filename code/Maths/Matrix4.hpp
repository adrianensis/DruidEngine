#pragma once

#include "Core/Core.hpp"
#include "Maths/Vector3.hpp"
#include "Maths/Vector4.hpp"

CLASS(Matrix4, ObjectBase)
{
private:
	STC bool smIdentityCreated;
	STC Matrix4 smIdentity;

	f32 mData[16];

	void setRows(CNS SArr<f32, 4> &row0, CNS SArr<f32, 4> &row1, CNS SArr<f32, 4> &row2, CNS SArr<f32, 4> &row3);
	void setRows(CNS f32 *row0, CNS f32 *row1, CNS f32 *row2, CNS f32 *row3);
	void setRows(u32 n);
	void setRows(CNS Vector4 &row0, CNS Vector4 &row1, CNS Vector4 &row2, CNS Vector4 &row3);

public:
	STC CNS Matrix4 &getIdentity();

	void init(u32 n);
	void init(CNS Matrix4 &other);
	void init(SArr<f32, 16> & data);
	void init(CNS SArr<f32, 4> &row0, CNS SArr<f32, 4> &row1, CNS SArr<f32, 4> &row2, CNS SArr<f32, 4> &row3);
	void init(CNS f32 *data);
	void init(CNS f32 *row0, CNS f32 *row1, CNS f32 *row2, CNS f32 *row3);
	void init(CNS Vector4 &row0, CNS Vector4 &row1, CNS Vector4 &row2, CNS Vector4 &row3);

	CNS f32 *getData() CNS;

	f32 get(u8 row, u8 col) CNS;
	void set(u8 row, u8 col, f32 value);

	void transpose();
	void invert();

	void mul(CNS Matrix4 &other);
	Vector4 mulVector(CNS Vector4 &vector) CNS;

	void zeros();
	void identity();
	void translation(CNS Vector3 &vector);
	void rotation(CNS Vector3 &vector);
	void scale(CNS Vector3 &vector);
	void ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
	void perspective(f32 near, f32 far, f32 aspect, f32 fov);
};
