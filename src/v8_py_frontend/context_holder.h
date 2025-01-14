#ifndef INCLUDE_MINI_RACER_CONTEXT_HOLDER_H
#define INCLUDE_MINI_RACER_CONTEXT_HOLDER_H

#include <v8.h>

namespace MiniRacer {

/** Create and manage a v8::Context */
class ContextHolder {
 public:
  explicit ContextHolder(v8::Isolate* isolate);
  ~ContextHolder();

  ContextHolder(const ContextHolder&) = delete;
  auto operator=(const ContextHolder&) -> ContextHolder& = delete;
  ContextHolder(ContextHolder&&) = delete;
  auto operator=(ContextHolder&& other) -> ContextHolder& = delete;

  auto Get() -> v8::Persistent<v8::Context>*;

 private:
  std::unique_ptr<v8::Persistent<v8::Context>> context_;
};

inline auto ContextHolder::Get() -> v8::Persistent<v8::Context>* {
  return context_.get();
}

}  // end namespace MiniRacer

#endif  // INCLUDE_MINI_RACER_CONTEXT_HOLDER_H
