/**
 *
 * @file    db.h
 * @author  damianzim
 * @date    2020-04-06
 *
 */

#ifndef KKV_STATUS_H_
#define KKV_STATUS_H_

#include <string>

#include "kkv/slice.h"

namespace kkv {

enum class State {
  kOk = 0,
  kError,

  kNotFound = 16,
  kNotExists,
  kAlreadyExists,

  kFatal = 128,

  kUnknown = 255,
};

using Result = State;

struct Status {
 public:
  Status() : state_(State::kOk), message_("") {};
  explicit Status(State state, Slice message = Slice()) : state_(state),
                                                          message_(message) {};

  inline void SetState(const State state) { state_ = state; message_ = ""; }
  inline void SetMessage(const Slice& message) { message_ = message; }

  inline void Set(State new_state, const Slice& new_message) {
    state_ = new_state;
    message_ = new_message;
  }

  [[nodiscard]] inline bool ok() const { return state_ == State::kOk; }
  inline bool IsState(const State state) { return state_ == state; }
  inline std::string ToString() { return message_.ToString(); }

 private:
  Slice message_;
  State state_;
};

} // namespace kkv

#endif  // KKV_STATUS_H_
