#ifndef DE_HASH_H
#define DE_HASH_H

#include "BasicTypes.hpp"
#include <string>

namespace DE {

/*!
  \brief Gives objects a hash number (u64).
*/
class Hash {

private:

  const u64 mHash = reinterpret_cast<u64>(this);

public:

  Hash() = default;
  virtual ~Hash() = default;

  virtual u64 hash() const{
    return mHash;
  };

  bool operator==(const Hash& rhs) const {
    return this->mHash == rhs.mHash;
  }

  bool operator!=(const Hash& rhs){
    return !((*this) == rhs);
  }

  template <class H>
  static u64 hash(H key){
	if(std::is_base_of<Hash,H>::value){
		return ((Hash*)&key)->hash();
	}else if(std::is_pointer<int*>::value){
		return (u64)key;
	}
  }

};


template <>
u64 Hash::hash<std::string>(std::string key);

 template <>
u64 Hash::hash<const c8*>(const c8* key);

template <>
u64 Hash::hash<u64>(u64 key);

template <>
u64 Hash::hash<u32>(u32 key);

template <>
u64 Hash::hash<i32>(i32 key);

template <>
u64 Hash::hash<i64>(i64 key);

template <>
u64 Hash::hash<u16>(u16 key);

template <>
u64 Hash::hash<i16>(i16 key);

template <>
u64 Hash::hash<u8>(u8 key);

template <>
u64 Hash::hash<i8>(i8 key);


} /* namespace DE */

#endif /* DE_HASH_H */
