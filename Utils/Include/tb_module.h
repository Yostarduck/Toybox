#pragma once

#include "tb_utils_prerequisites.h"

namespace toyboxSDK {

/**
* Description.
*
* Example usage.
* 
*/
template<class T>
class TB_UTIL_EXPORT Module
{
 public:
  
  /**
  * Description.
  */
  static T&
  instance() {
    if (isShutDown()) {
      throw "Trying to access a module but it hasn't been started.";
    }

    if (isDestroyed()) {
      throw "Trying to access a destroyed module.";
    }
    return *_instance();
  }

  /**
  * Description.
  */
  static T*
  instancePtr() {
    if (isShutDown()) {
      throw "Trying to access a module but it hasn't been started.";
    }

    if (isDestroyed()) {
      throw "Trying to access a destroyed module.";
    }
    return _instance();
  }

  /**
  * Description.
  */
  template<class... Args> static void
  startUp(Args&& ...args) {
    if (!isShutDown()) {
      throw "Trying to start an already started module.";
    }

    _instance() = new T(std::forward<Args>(args)...);
    isShutDown() = false;

    static_cast<Module*>(_instance())->onStartUp();
  }

  /**
  * Description.
  */
  template<class SubType, class... Args> static void
  startUp(Args&& ...args) {
    static_assert(std::is_base_of<T, SubType>::value,
      "Provided type isn't derived from type the Module is initialized with.");

    if (!isShutDown()) {
      throw "Trying to start an already started module.";
    }

    _instance() = new SubType(std::forward<Args>(args)...);
    isShutDown() = false;

    static_cast<Module*>(_instance())->onStartUp();
  }

  /**
  * Description.
  */
  static void
  shutDown() {
    if (isShutDown() || isDestroyed()) {
      throw "Trying to shut down an already shut down module.";
    }

    static_cast<Module*>(_instance())->onShutDown();

    delete(_instance());
    isShutDown() = true;
  }

  /**
  * Description.
  */
  static bool
  isStarted() {
    return !isShutDown() && !isDestroyed();
  }

 protected:
  
  /**
  * Description.
  */
  Module() = default;

  /**
  * Description.
  */
  virtual ~Module() {
    _instance() = nullptr;
    isDestroyed() = true;
  }

  /**
  * Description.
  */
  Module(const Module&) = delete;

  /**
  * Description.
  */
  Module(Module&&) = delete;

  /**
  * Description.
  */
  Module&
  operator=(const Module&) = delete;

  /**
  * Description.
  */
  Module&
  operator=(Module&&) = delete;

  /**
  * Description.
  */
  virtual void
  onStartUp() {}

  /**
  * Description.
  */
  virtual void
  onShutDown() {}

  /**
  * Description.
  */
  static T*&
  _instance() {
    static T* inst = nullptr;
    return inst;
  }

  /**
  * Description.
  */
  static bool&
  isDestroyed() {
    static bool inst = false;
    return inst;
  }

  /**
  * Description.
  */
  static bool&
  isShutDown() {
    static bool inst = true;
    return inst;
  }
};

}