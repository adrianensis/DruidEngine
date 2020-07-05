#ifndef DE_MATRIX4_H
#define DE_MATRIX4_H

#include "DE_Class.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

namespace DE {

template<class T> class Array;

class Matrix4: public DE_Class /*16 bytes alignment*/{
private:

	static bool smIdentityCreated;
	static Matrix4 smIdentity;

	f32 mData[16];

	void setRows(const Array<f32> &row0, const Array<f32> &row1, const Array<f32> &row2, const Array<f32> &row3);
	void setRows(const f32 *row0, const f32 *row1, const f32 *row2, const f32 *row3);
	void setRows(u32 n);
	void setRows(const Vector4 &row0, const Vector4 &row1, const Vector4 &row2, const Vector4 &row3);

public:

	DE_CLASS(Matrix4, DE_Class)

	static const Matrix4& getIdentity();

	void init(u32 n);
	void init(const Matrix4 &other);
	void init(Array<f32> &data);
	void init(const Array<f32> &row0, const Array<f32> &row1, const Array<f32> &row2, const Array<f32> &row3);
	void init(const f32 *data);
	void init(const f32 *row0, const f32 *row1, const f32 *row2, const f32 *row3);
	void init(const Vector4 &row0, const Vector4 &row1, const Vector4 &row2, const Vector4 &row3);

	const f32* getData() const;

	f32 get(u8 row, u8 col) const;
	void set(u8 row, u8 col, f32 value);

	void transpose();
	void invert();

	void mul(const Matrix4 &other);
	Vector4 mulVector(const Vector4 &vector);

	void zeros();
	void identity();
	void translation(const Vector3 &vector);
	void rotation(const Vector3 &vector);
	void scale(const Vector3 &vector);
	void ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
	void perspective(f32 near, f32 far, f32 aspect, f32 fov);
};

} /* namespace DE */

#endif /* DE_MATRIX4_H */
