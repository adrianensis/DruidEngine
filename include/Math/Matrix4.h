#ifndef MATRIX4_H_
#define MATRIX4_H_

#include "Basic.h"
#include "Array.h"

namespace DE {

DE_CLASS(Matrix4) /*16 bytes alignment*/ {

  // REMEMBER
  // OpenGL uses column-major order (transposed)
  // a GL matrix : [ [column 0] [column 1] [column 2] [column 3] ]

  // NOTE:
  // You can use the get(row,col) and set(row,col,value) functions
  // to access the data as a row-major ordered matrix.

  // NOTE:
  // Here I use, column-major order.

  /*
      ROW-MAJOR ORDER:

      row 0 | a b c d |
      row 1 | e f g h |
      row 2 | i j k l |
      row 3 | m n o p |

      COLUMN-MAJOR ORDER:

      column 0 | a e i m |
      column 1 | b f j n |
      column 2 | c g k o |
      column 3 | d h l p |

  */

private:

    f32 mData[16];

    void setRows(const Array<f32>& row0, const Array<f32>& row1, const Array<f32>& row2, const Array<f32>& row3);
    void setRows(const f32* row0, const f32* row1, const f32* row2, const f32* row3);
    void setRows(f32 n);

public:

    Matrix4();
    virtual ~Matrix4();
    void init(f32 n);
    void init(const Matrix4& other);
    void init(Array<f32>& data);
    void init(const Array<f32>& row0, const Array<f32>& row1, const Array<f32>& row2, const Array<f32>& row3);
    void init(const f32* data);
    void init(const f32* row0, const f32* row1, const f32* row2, const f32* row3);
    f32* getData();
    f32 get(u8 row, u8 col);
    void set(u8 row, u8 col, f32 value);
    void transpose();
    void transpose(Matrix4& other);

};

} /* namespace DE */

#endif /* MATRIX4_H_ */
