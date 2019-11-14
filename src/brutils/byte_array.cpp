//
// Created by congay on 13/11/2019.
//

#include "brutils/byte_array.h"

#include <algorithm>

using namespace brutils;

byte_array::byte_array() = default;
byte_array::byte_array(const byte_array &other) = default;
byte_array::byte_array(byte_array &&other) noexcept = default;
byte_array::byte_array(std::size_t size) : _byteVector(size) {}
byte_array::byte_array(std::size_t size, std::byte byte) : _byteVector(size, byte) {}
byte_array::byte_array(std::size_t size, char byte) : _byteVector(size, std::byte(byte)) {}
byte_array::byte_array(const char *data, std::size_t size)
{
  std::string_view tmp(data, size);
  std::transform(tmp.begin(), tmp.end(), _byteVector.end(), [] (const char c) {
    return std::byte(c);
  });
}
byte_array &byte_array::append(const byte_array &ba)
{
  _byteVector.reserve(this->_byteVector.size()+ba._byteVector.size());
  _byteVector.insert(_byteVector.end(), ba._byteVector.begin(), ba._byteVector.end());
  return *this;
}
byte_array &byte_array::append(char c)
{
  _byteVector.push_back(std::byte(c));
  return *this;
}
byte_array &byte_array::append(int count, char c)
{
  _byteVector.reserve(this->_byteVector.size()+count);
  for (int i = 0; i < count; ++i) {
    _byteVector.push_back(std::byte(c));
  }
  return *this;
}
byte_array &byte_array::append(const char *str)
{
  std::string_view tmp = str;
  std::transform(tmp.begin(), tmp.end(), _byteVector.end(), [] (unsigned char c){
    return std::byte(c);
  });
  return *this;
}
byte_array &byte_array::append(const std::string &str)
{
  std::transform(str.cbegin(), str.cend(), _byteVector.end(), [] (unsigned char c) {
    return std::byte(c);
  });
}
byte_array &byte_array::insert(size_t pos, const byte_array &ba)
{
  for (auto i = 0; i < ba._byteVector.size(); ++i) {
    _byteVector.insert()
    _byteVector.insert(pos, ba._byteVector.at(i));
  }
}