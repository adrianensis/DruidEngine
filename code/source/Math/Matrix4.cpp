#include "Matrix4.h"
#include "MathUtils.h"

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

void Matrix4::setRows(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3){
  Vector4 rows[4] = {row0,row1,row2,row3};

  for (u8 row = 0; row < 4; row++)
    for (u8 col = 0; col < 4; col++)
      Matrix4::set(row,col,rows[row][col]);
};

Matrix4::Matrix4() : DE_Class(){
}

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

void Matrix4::init(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3){
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
  Matrix4 copy;
  copy.init((*this));

  for (u8 row = 0; row < 4; row++)
    for (u8 col = 0; col < 4; col++)
      this->set(col,row,copy.get(row,col));
};

void Matrix4::mul(const Matrix4& other){
  Matrix4 copy;
  copy.init((*this));

  this->setRows(0);

  for (u32 i = 0; i < 4; i++)
      for (u32 j = 0; j < 4; j++)
          for (u32 k = 0; k < 4; k++)
              this->set(i,j,this->get(i,j) + copy.get(i,k) * other.get(k,j));
};


Vector4 Matrix4::mulVector(const Vector4& vector){
  Vector4 result(0,0,0,0);

  for (u32 row = 0; row < 4; row++){
      result[row] = 0;
      for (u32 col = 0; col < 4; col++)
          result[row] += this->get(row,col) * vector[col];
  }

  return result;
};


Matrix4 Matrix4::zeros(){
  Matrix4 m;
  m.init(0.0f);
  return m;
};

Matrix4 Matrix4::identity(){
  Matrix4 m;
  m.init(0.0f);
  m.set(0,0,1.0f);
  m.set(1,1,1.0f);
  m.set(2,2,1.0f);
  m.set(3,3,1.0f);
  return m;
};

Matrix4 Matrix4::translation(const Vector3& vector){
  Matrix4 m = Matrix4::identity();
  m.set(0,3,vector.x);
  m.set(1,3,vector.y);
  m.set(2,3,vector.z);
  return m;
};

Matrix4 Matrix4::rotation(const Vector3& vector){
  Matrix4 m = Matrix4::identity();

  f32 radians, cos, sin;

  if(vector.x != 0){
      radians = MathUtils::rad(vector.x);
      sin = sinf(radians);
      cos = cosf(radians);
      m.set(1,1,cos);
      m.set(1,2,-sin);
      m.set(2,1,sin);
      m.set(2,2,cos);
  }else if(vector.y != 0){
      radians = MathUtils::rad(vector.y);
      sin = sinf(radians);
      cos = cosf(radians);
      m.set(0,0,cos);
      m.set(0,2,sin);
      m.set(2,0,-sin);
      m.set(2,2,cos);
  }else if(vector.z != 0){
      radians = MathUtils::rad(vector.z);
      sin = sinf(radians);
      cos = cosf(radians);
      m.set(0,0,cos);
      m.set(0,1,-sin);
      m.set(1,0,sin);
      m.set(1,1,cos);
  }

  return m;
};

Matrix4 Matrix4::scale(const Vector3& vector){
  Matrix4 m = Matrix4::identity();
  m.set(0,0,vector.x);
  m.set(1,1,vector.y);
  m.set(2,2,vector.z);
  return m;
};

Matrix4 Matrix4::ortho(const f32 left, const f32 right, const f32 bottom, const f32 top, const f32 near, f32 far){
  Matrix4 m = Matrix4::identity();
  m.set(0,0,2.0f/(right-left));
  m.set(0,3,-((right+left)/(right-left)));
  m.set(1,1,2.0f/(top-bottom));
  m.set(1,3,-((top+bottom)/(top-bottom)));
  m.set(2,2,-2.0f/(far-near));
  m.set(2,3,-((far+near)/(far-near)));
  return m;
};

Matrix4 Matrix4::perspective(const f32 near, const f32 far, const f32 aspect, f32 fov){
  f32 top = near * tanf((fov/2)*MathUtils::PI_180);
  f32 bottom = -top;
  f32 right = top*aspect;
  f32 left = -right;

  Matrix4 m = Matrix4::zeros();
  m.set(0,0,(2*near)/(right-left));
  m.set(0,2,(right+left)/(right-left));
  m.set(1,1,(2*near)/(top-bottom));
  m.set(1,2,(top+bottom)/(top-bottom));
  m.set(2,2,(-(far+near))/(far-near));
  m.set(2,3,(-2*far*near)/(far-near));
  m.set(3,2,-1.0f);
  return m;
};


} /* namespace DE */
