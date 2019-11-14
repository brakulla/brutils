//
// Created by congay on 13/11/2019.
//

#ifndef BRUTILS_INCLUDE_BRUTILS_BYTEARRAY_H_
#define BRUTILS_INCLUDE_BRUTILS_BYTEARRAY_H_

#include <string>
#include <vector>

namespace brutils
{

class byte_array
{
 public:
  byte_array();
  byte_array(const byte_array &other);
  byte_array(byte_array &&other) noexcept;
  explicit byte_array(std::size_t size);
  byte_array(std::size_t size, std::byte byte);
  byte_array(std::size_t size, char byte);
  explicit byte_array(const char *data, std::size_t size = -1);

  byte_array &append(const byte_array &ba);
  byte_array &append(char c);
  byte_array &append(int count, char c);
  byte_array &append(const char *str);
  byte_array &append(const char *str, std::size_t len);
  byte_array &append(const std::string &str);

  byte_array &insert(size_t pos, const byte_array &ba);
  byte_array &insert(size_t pos, char c);
  byte_array &insert(size_t pos, const std::string &str);
  byte_array &insert(size_t pos, const char *str);
  byte_array &insert(size_t pos, const char *str, size_t len);

  void push_back(const byte_array &ba);
  void push_back(char c);
  void push_back(const char *str);
  void push_back(const std::string &str);

  void clear();

  std::byte at(std::size_t pos);
  std::byte back() const;
  std::byte front() const;
  std::byte* data();

  std::vector<std::byte>::iterator begin(); // TODO: change return type
  std::vector<std::byte>::const_iterator cbegin();
  std::vector<std::byte>::iterator end(); // TODO: change return type
  std::vector<std::byte>::const_iterator cend();

  bool isEmpty() const;
  size_t size() const;
  void reserve(size_t size);
  void resize(size_t size);
  void shrink_to_fit();
  size_t capacity();

  int compare(const char *str, bool caseSensitive = false);
  int compare(std::string str, bool caseSensitive = false);
  int compare(const byte_array &ba, bool caseSensitive = false);

  bool isLower() const;
  bool isUpper() const;
  bool contains(const byte_array &ba) const;
  bool contains(char c) const;
  bool contains(const char *str) const;
  bool contains(const std::string &str) const;
  bool startsWith(const byte_array &ba) const;
  bool startsWith(char c) const;
  bool startsWith(const char *str) const;
  bool startsWith(const std::string &str) const;

  size_t indexOf(const byte_array& ba, size_t from = 0);
  size_t indexOf(char c, size_t from = 0);
  size_t indexOf(const char *str, size_t from = 0);
  size_t indexOf(const std::string &str, size_t from = 0);
  size_t lastIndexOf(const byte_array& ba, size_t from = 0);
  size_t lastIndexOf(char c, size_t from = 0);
  size_t lastIndexOf(const char *str, size_t from = 0);
  size_t lastIndexOf(const std::string &str, size_t from = 0);

  short toShort(bool *ok = nullptr, int base = 10) const;
  unsigned short toUShort(bool *ok = nullptr, int base = 10) const;
  int toInt(bool *ok = nullptr, int base = 10) const;
  unsigned int toUInt(bool *ok = nullptr, int base = 10) const;
  long toLong(bool *ok = nullptr, int base = 10) const;
  unsigned long toULong(bool *ok = nullptr, int base = 10) const;
  long long toLongLong(bool *ok = nullptr, int base = 10) const;
  unsigned long long toULongLong(bool *ok = nullptr, int base = 10) const;
  std::string toString() const;
  byte_array toLower() const;
  byte_array toUpper() const;

  friend void swap(byte_array& first, byte_array& second)
  {
    std::swap(first._byteVector, second._byteVector);
  }

 private:
  std::vector<std::byte> _byteVector;
};

}

#endif //BRUTILS_INCLUDE_BRUTILS_BYTEARRAY_H_
