#ifndef DE_MATRIX4_H
#define DE_MATRIX4_H

#include "Array.h"
#include "Vector4.h"
#include "Vector3.h"

namespace DE {

class Matrix4 : public DE_Class /*16 bytes alignment*/ {
private:

  f32 mData[16];

  void setRows(const Array<f32>& row0, const Array<f32>& row1, const Array<f32>& row2, const Array<f32>& row3);
  void setRows(const f32* row0, const f32* row1, const f32* row2, const f32* row3);
  void setRows(const f32 n);
  void setRows(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3);

public:

  Matrix4();
  ~Matrix4();
  void init(const f32 n);
  void init(const Matrix4& other);
  void init(Array<f32>& data);
  void init(const Array<f32>& row0, const Array<f32>& row1, const Array<f32>& row2, const Array<f32>& row3);
  void init(const f32* data);
  void init(const f32* row0, const f32* row1, const f32* row2, const f32* row3);
  void init(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3);

  const f32* getData() const;

  f32 get(const u8 row, const u8 col) const;
  void set(const u8 row, const u8 col, const f32 value);

  void transpose();

  void mul(const Matrix4& other);
  Vector4 mulVector(const Vector4& vector);

  static Matrix4 zeros();
  static Matrix4 identity();
  static Matrix4 translation(const Vector3& vector);
  static Matrix4 rotation(const Vector3& vector);
  static Matrix4 scale(const Vector3& vector);
  static Matrix4 ortho(const f32 left, const f32 right, const f32 bottom, const f32 top, const f32 near, f32 far);
  static Matrix4 perspective(const f32 near, const f32 far, const f32 aspect, f32 fov);
};

} /* namespace DE */

#endif /* DE_MATRIX4_H */
