#include "Hash.hpp"

namespace DE {

  template <>
  u64 Hash::hash<std::string>(std::string key){
  std::hash<std::string> hash_fn;
  u64 hashString = hash_fn(key);
  return hashString;
  }

   template <>
  u64 Hash::hash<const c8*>(const c8* key){
  std::string str(key);
    std::hash<std::string> hash_fn;
    u64 hashString = hash_fn(str);
    return hashString;
  }

  template <>
  u64 Hash::hash<u64>(u64 key){
  return key;
  }

  template <>
  u64 Hash::hash<u32>(u32 key){
  return static_cast<u64>(key);
  }

  template <>
  u64 Hash::hash<i32>(i32 key){
  return static_cast<u64>(key);
  }

  template <>
  u64 Hash::hash<i64>(i64 key){
  return static_cast<u64>(key);
  }

  template <>
  u64 Hash::hash<u16>(u16 key){
  return static_cast<u64>(key);
  }

  template <>
  u64 Hash::hash<i16>(i16 key){
  return static_cast<u64>(key);
  }

  template <>
  u64 Hash::hash<u8>(u8 key){
  return static_cast<u64>(key);
  }

  template <>
  u64 Hash::hash<i8>(i8 key){
  return static_cast<u64>(key);
  }

  // ---------------------------------------------------------------------------

} /* namespace DE */
