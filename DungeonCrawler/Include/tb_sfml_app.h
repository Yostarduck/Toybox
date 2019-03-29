/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
/**
 * @file tb_sfml_app.h
 * @author Marco "Swampy" Millan
 * @date 2019/03/18 2019
 * @brief SFML Application
 * 
 */
/*||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||°°°||*/
#pragma once

#include <tb_core_prerequisites.h>
#include <tb_module.h>

#include <SFML/Graphics.hpp>

#include "tb_dungeon.h"

namespace toyboxSDK {
  using namespace sf;



  class SFMLApp : Module<SFMLApp>
  {
  public:
    /**
     * Default constructor
     */
    SFMLApp() = default;
    
    /**
     * Default destructor
     */
    ~SFMLApp() = default;


    /**
     * Runs the game
     */
    bool
    run();

 
  protected:

    /**
     *
     */
    void
    postInit();

    /**
     *
     */
    void
    postUpdate();

    /**
     *
     */
    virtual void
      postRender() = 0;

    /**
     *
     */
    virtual void
      postDestroy() = 0;

    /**
     *
     */
    virtual void
      onResize() = 0;

  private:

    /**
     *
     */
    void
    init();

    /**
     *
     */
    void
    createWindow();

    /**
    *
    */
    void
    update();

    /**
    *
    */
    void
    render();

    /**
    *
    */
    void
    destroy();


   private:

    /**
     *
     */
    TString m_AppName;
    TString m_AppPath;

    bool m_usesWindow;



  };
  
}

