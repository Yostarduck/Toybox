#pragma once

#include "tb_utils_prerequisites.h"

namespace toyboxSDK {

/**
* Description.
*
* Example usage.
*
*/
class Command {
 public:

  /**
  * Class default constructor.
  */
  Command() {};

  /**
  * Class default destructor.
  */
  virtual ~Command() {};

  /**
  * Description.
  */
  virtual void
  execute() = 0;
  
  /**
  * Description.
  */
  virtual void
  undo() = 0;
  
  /**
  * Description.
  */
  virtual void
  redo() = 0;

 private:

 protected:

};

}