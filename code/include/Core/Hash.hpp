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
  
  template <>
  static u64 hash<std::string>(std::string key){
	std::hash<std::string> hash_fn;
	u64 hashString = hash_fn(key);
	return hashString;
  }
  
   template <>
  static u64 hash<const c8*>(const c8* key){
	std::string str(key);
    std::hash<std::string> hash_fn;
    u64 hashString = hash_fn(str);
    return hashString;
  }
  
  template <>
  static u64 hash<u64>(u64 key){
	return key;
  }

  template <>
  static u64 hash<u32>(u32 key){
	return static_cast<u64>(key);
  }
  
  template <>
  static u64 hash<i32>(i32 key){
	return static_cast<u64>(key);
  }

  template <>
  static u64 hash<i64>(i64 key){
	return static_cast<u64>(key);
  }

  template <>
  static u64 hash<u16>(u16 key){
	return static_cast<u64>(key);
  }

  template <>
  static u64 hash<i16>(i16 key){
	return static_cast<u64>(key);
  }
  
  template <>
  static u64 hash<u8>(u8 key){
	return static_cast<u64>(key);
  }
  
  template <>
  static u64 hash<i8>(i8 key){
	return static_cast<u64>(key);
  }

  // generic function: arithmetic keys and pointers.
/*   static u64 hash(u64 key){
    return key;
  }; */

/*   static u64 hash(const byte* key){
    return reinterpret_cast<u64>(key); // calls hash(const u64 key) const.
  };
 */
/*   static u64 hash(f32 key){
    return static_cast<u64>(key); // calls hash(const u64 key) const.
  };
 */
  /* static u64 hash(u8 key){
    return static_cast<u64>(key); // calls hash(const u64 key) const.
  };

  static u64 hash(u16 key){
    return static_cast<u64>(key); // calls hash(const u64 key) const.
  };
 */
/*   static u64 hash(u32 key){
    return static_cast<u64>(key); // calls hash(const u64 key) const.
  }; */
/* 
  static u64 hash(i8 key){
    return static_cast<u64>(key); // calls hash(const u64 key) const.
  };

  static u64 hash(i16 key){
    return static_cast<u64>(key); // calls hash(const u64 key) const.
  };

  static u64 hash(i32 key){
    return static_cast<u64>(key); // calls hash(const u64 key) const.
  };

  static u64 hash(i64 key){
    return static_cast<u64>(key); // calls hash(const u64 key) const.
  }; */

  // for Hash objects
/*   static u64 hash(Hash key){
    return key.hash(); // calls hash(const u64 key) const.
  }; */
  
  // for Hash pointers
/*   static u64 hash(const Hash* key){
    return key->hash(); // calls hash(const u64 key) const.
  };
 */
  // for strings
/*   static u64 hash(const std::string& key){
    std::hash<std::string> hash_fn;
    u64 hashString = hash_fn(key);
    return hashString; // calls hash(const u64 key) const.
  }; */

/*   static u64 hash(const char* key){
    std::string str(key);
    std::hash<std::string> hash_fn;
    u64 hashString = hash_fn(str);
    return hashString; // calls hash(const u64 key) const.
  }; */
};

} /* namespace DE */

#endif /* DE_HASH_H */
