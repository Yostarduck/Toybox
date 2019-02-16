#pragma once

#include "tb_utils_prerequisites.h"

#include <memory>
#include <functional>

namespace toyboxSDK{

template<class T, class... Args>
std::unique_ptr<T>
tb_make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template<class T>
void
tb_simple_deleter(void* data) {
  T* info = static_cast<T*>(data);
  delete info;
}

////////////////////////////////////
//Custom version of smart pointers//
////////////////////////////////////

template<class T, class Deleter>
std::unique_ptr<T, Deleter>
tb_unique_custom(T* ptr, Deleter deleter) {
  return std::unique_ptr<T, Deleter>(ptr, deleter);
}

template<class T, class Deleter>
std::shared_ptr<T>
tb_shared_custom(T* ptr, Deleter deleter) {
  return std::shared_ptr<T>(ptr, deleter);
}

///////////////////////////////////////////
//Simple version of custom smart pointers//
///////////////////////////////////////////


template<class T>
using SimpleUnique = std::unique_ptr<T, std::function<void(T*)>>;

template<class T>
using SimpleShared = std::shared_ptr<T>;

template<class T>
SimpleUnique<T>
tb_simple_unique(T* ptr) {
  return tb_unique_custom(ptr, &tb_simple_deleter<T>);
}

template<class T>
SimpleShared<T>
tb_simple_shared(T* ptr) {
  return tb_shared_custom(ptr, &tb_simple_deleter<T>);
}

}