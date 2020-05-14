/**
 *
 * @file    slice.h
 * @author  damianzim
 * @date    2020-04-28
 *
 */

#ifndef KKV_SLICE_H_
#define KKV_SLICE_H_

#include <stdio.h>
#include <string.h>

#include <string>

#include <spdlog/fmt/ostr.h>

namespace kkv {

class Slice {
 public:
  Slice() : s_(""), len_(0) {};
  Slice(const char* s, size_t len) : s_(s), len_(len) {
    if (len_ == 0 || s_ == nullptr) {
      s_ = "";
      len_ = 0;
    }
  }
  Slice(const char* s) : s_(s), len_(strlen(s)) {};
  Slice(const std::string& s) : s_(s.data()), len_(s.size()) {};

  inline const char* Begin() const { return s_; }
  inline const char* End() const { return s_ + len_; }

  inline const char* Data() const { return s_; }
  inline std::string ToString() const { return std::string(Begin(), Size()); }

  inline Slice Copy() const { return *this; }

  inline size_t Size() const { return len_; }
  inline bool IsEmpty() const { return len_ == 0; }

  // Get the slice value from indexes <start; len_of_slice - 1>.
  inline Slice Substr(size_t start) const {
    if (Size() == 0 || start >= Size())
      return Slice();

    return Slice(Begin() + start);

  }

  // Get the slice value from indexes <start; end) of slice.
  inline Slice Substr(size_t start, size_t end) const {
    if (start > end || Size() == 0 || start >= Size() )
      return Slice();

    auto slice_len = (end > Size() ? Size() : end)  - start;
    return Slice(Begin() + start, slice_len);
  };

  inline Slice Left(size_t size) const {
    return size > Size() ? *this : Slice(Begin(), size);
  }

  inline Slice Right(size_t size) const {
    return size > Size() ? *this : Slice(Begin() + Size() - size, size);
  }

  inline bool HasPrefix(Slice& prefix) const {
    if (prefix.Size() == 0)
      return true;
    else if (prefix.Size() > Size())
      return false;

    return strncmp(Data(), prefix.Data(), prefix.Size()) == 0;
  };

  bool HasSuffix(Slice& suffix) const {
    if (suffix.Size() == 0)
      return true;
    else if (suffix.Size() > Size())
      return false;

    return (strncmp(Begin() + Size() - suffix.Size(), suffix.Data(),
                    suffix.Size()) == 0);
  }

  // Use data by default while logging Slice.
  template<typename OStream>
  friend OStream& operator<<(OStream& os, const Slice& slice) {
    return os << slice.Data();
  }

 private:
  const char * s_;

  size_t len_;
};

inline bool operator==(const Slice& a, const Slice& b) {
  return (a.Size() == b.Size()) && !memcmp(a.Data(), b.Data(), a.Size());
}

inline bool operator!=(const Slice& a, const Slice& b) { return !(a == b); }

} // namespace kkv

#endif  // KKV_SLICE_H_
