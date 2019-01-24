#include "tb_graphics_prerequisites.h"
#include "tb_graphics_defines.h"
#include <tb_module.h>

namespace toyboxSDK {

//TODO: Implement this classes
class Device;
class DeviceContext;
class RenderTarget;
class SwapChain;
class Texture;
class BlendState;
class DepthStencil;
class DepthStencilState;
class RasterizerState;

class TB_GRAPHICS_EXPORT GraphicsAPI : public Module<GraphicsAPI> {
 public:
  /*
  * Default class constructor.
  */
  GraphicsAPI();

  /*
  * Default class destructor.
  */
  ~GraphicsAPI();

  /**
  * Inherited via Module.
  */
  virtual void
  onStartUp() override;

  /**
  * Inherited via Module.
  */
  virtual void
  onShutDown() override;

  /*
  * Returns which graphic is being used.
  * 
  * @return
  *   m_SelectedGraphicsAPI
  */
  FORCEINLINE TB_GRAPHICS_API::E
  selectedGraphicAPI() {
    return m_SelectedGraphicAPI;
  }

 protected:
  
 private:
  TB_GRAPHICS_API::E m_SelectedGraphicAPI;
};

}