#include "Matrix4.h"
#include "MathUtils.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Array.h"

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

  FOR_RANGE(row, 0, 4)
    FOR_RANGE(col, 0, 4)
      Matrix4::set(row,col,rows[row][col]);
};

void Matrix4::setRows(const f32* row0, const f32* row1, const f32* row2, const f32* row3){
  const f32* rows[4] = {row0,row1,row2,row3};

  FOR_RANGE(row, 0, 4)
    FOR_RANGE(col, 0, 4)
      Matrix4::set(row,col,rows[row][col]);
};

void Matrix4::setRows(u32 n){
  FOR_RANGE(row, 0, 4)
    FOR_RANGE(col, 0, 4)
      Matrix4::set(row,col,n);
};

void Matrix4::setRows(const Vector4& row0, const Vector4& row1, const Vector4& row2, const Vector4& row3){
  Vector4 rows[4] = {row0,row1,row2,row3};

  FOR_RANGE(row, 0, 4)
    FOR_RANGE(col, 0, 4)
      Matrix4::set(row,col,rows[row][col]);
};

Matrix4::Matrix4() : DE_Class(){
}

Matrix4::~Matrix4() = default;

void Matrix4::init(u32 n){
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

f32 Matrix4::get(u8 row, u8 col) const{
  return mData[row+(4*col)];
};

void Matrix4::set(u8 row, u8 col, f32 value){
  mData[row+(4*col)] = value;
};

void Matrix4::transpose(){
  Matrix4 copy;
  copy.init((*this));

  FOR_RANGE(row, 0, 4)
    FOR_RANGE(col, 0, 4)
      this->set(col,row,copy.get(row,col));
};

void Matrix4::mul(const Matrix4& other){
  Matrix4 copy;
  copy.init((*this));

  this->setRows(0);

  FOR_RANGE (i, 0, 4)
    FOR_RANGE (j, 0, 4)
        FOR_RANGE (k, 0, 4)
              this->set(i,j,this->get(i,j) + copy.get(i,k) * other.get(k,j));
};


Vector4 Matrix4::mulVector(const Vector4& vector){
  Vector4 result(0,0,0,0);

  FOR_RANGE (row, 0, 4){
    result[row] = 0;
    FOR_RANGE (col, 0, 4)
          result[row] += this->get(row,col) * vector[col];
  }

  return result;
};


void Matrix4::zeros(){
  this->init(0.0f);
};

void Matrix4::identity(){
  this->init(0.0f);
  this->set(0,0,1.0f);
  this->set(1,1,1.0f);
  this->set(2,2,1.0f);
  this->set(3,3,1.0f);
};

void Matrix4::translation(const Vector3& vector){
  this->identity();
  this->set(0,3,vector.x);
  this->set(1,3,vector.y);
  this->set(2,3,vector.z);
};

void Matrix4::rotation(const Vector3& vector){

  this->identity();

  f32 radians, cos, sin;

  if(vector.x != 0){
      radians = MathUtils::rad(vector.x);
      sin = sinf(radians);
      cos = cosf(radians);
      this->set(1,1,cos);
      this->set(1,2,-sin);
      this->set(2,1,sin);
      this->set(2,2,cos);
  }else if(vector.y != 0){
      radians = MathUtils::rad(vector.y);
      sin = sinf(radians);
      cos = cosf(radians);
      this->set(0,0,cos);
      this->set(0,2,sin);
      this->set(2,0,-sin);
      this->set(2,2,cos);
  }else if(vector.z != 0){
      radians = MathUtils::rad(vector.z);
      sin = sinf(radians);
      cos = cosf(radians);
      this->set(0,0,cos);
      this->set(0,1,-sin);
      this->set(1,0,sin);
      this->set(1,1,cos);
  }

};

void Matrix4::scale(const Vector3& vector){
  this->identity();
  this->set(0,0,vector.x);
  this->set(1,1,vector.y);
  this->set(2,2,vector.z);
};

void Matrix4::ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far){
  this->identity();
  this->set(0,0,2.0f/(right-left));
  this->set(0,3,-((right+left)/(right-left)));
  this->set(1,1,2.0f/(top-bottom));
  this->set(1,3,-((top+bottom)/(top-bottom)));
  this->set(2,2,-2.0f/(far-near));
  this->set(2,3,-((far+near)/(far-near)));
};

void Matrix4::perspective(f32 near, f32 far, f32 aspect, f32 fov){
  f32 top = near * tanf((fov/2)*MathUtils::PI_180);
  f32 bottom = -top;
  f32 right = top*aspect;
  f32 left = -right;

  this->identity();
  this->set(0,0,(2*near)/(right-left));
  this->set(0,2,(right+left)/(right-left));
  this->set(1,1,(2*near)/(top-bottom));
  this->set(1,2,(top+bottom)/(top-bottom));
  this->set(2,2,(-(far+near))/(far-near));
  this->set(2,3,(-2*far*near)/(far-near));
  this->set(3,2,-1.0f);
};


} /* namespace DE */
