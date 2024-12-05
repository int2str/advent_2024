#ifndef CURRY_HH
#define CURRY_HH

#include <utility>
#include <functional>

namespace Utils {

template <typename FN>
auto uncurry(FN fun) {
  return [fun_ = std::move(fun)](auto&& tuple) {
    return std::apply(fun_, std::forward<decltype(tuple)>(tuple));
  };
}

}

#endif  // CURRY_HH
