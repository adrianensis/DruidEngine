#include "Matrix4.h"

#include <cstring>

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


namespace DE {

void Matrix4::setRows(const Array<f32>& row0, const Array<f32>& row1, const Array<f32>& row2, const Array<f32>& row3){

  Array<f32> rows[4] = {row0,row1,row2,row3};

  for (u8 row = 0; row < 4; row++)
  for (u8 col = 0; col < 4; col++)
  Matrix4::set(row,col,rows[row][col]);
};

void Matrix4::setRows(const f32* row0, const f32* row1, const f32* row2, const f32* row3){
  const f32* rows[4] = {row0,row1,row2,row3};

  for (u8 row = 0; row < 4; row++)
  for (u8 col = 0; col < 4; col++)
  Matrix4::set(row,col,rows[row][col]);
};

void Matrix4::setRows(const f32 n){
  for (u8 row = 0; row < 4; row++)
  for (u8 col = 0; col < 4; col++)
  Matrix4::set(row,col,n);
};

Matrix4::Matrix4() = default;

Matrix4::~Matrix4() = default;

void Matrix4::init(const f32 n){
  Matrix4::setRows(n);
};

void Matrix4::init(const Matrix4& other){
  Matrix4::init(other.mData);
};

void Matrix4::init(Array<f32>& data){
  memcpy(mData,&data[0],16*sizeof(f32));
};

void Matrix4::init(const Array<f32>& row0, const Array<f32>& row1, const Array<f32>& row2, const Array<f32>& row3){
  Matrix4::setRows(row0,row1,row2,row3);
};

void Matrix4::init(const f32* data){
  memcpy(mData,data,16*sizeof(f32));
};

void Matrix4::init(const f32* row0, const f32* row1, const f32* row2, const f32* row3){
  Matrix4::setRows(row0,row1,row2,row3);
};

const f32* Matrix4::getData() const{
  return mData;
};

f32 Matrix4::get(const u8 row, const u8 col) const{
  return mData[row+(4*col)];
};

void Matrix4::set(const u8 row, const u8 col, const f32 value){
  mData[row+(4*col)] = value;
};

void Matrix4::transpose(){
  Matrix4::transpose((*this));
};

void Matrix4::transpose(Matrix4& destiny){
  Matrix4 copy;
  copy.init((*this));

  for (u8 row = 0; row < 4; row++)
  for (u8 col = 0; col < 4; col++)
  destiny.set(col,row,copy.get(row,col));

};

} /* namespace DE */
