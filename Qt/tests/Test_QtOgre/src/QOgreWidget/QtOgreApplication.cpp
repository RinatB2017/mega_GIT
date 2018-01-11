/**********************************************************************************************************************
**         __________              ___                              ________                                         **
**         \______   \_____     __| _/ _____  _____     ____       /  _____/ _____     _____    ____    ______       **
**          |       _/\__  \   / __ | /     \ \__  \   /    \     /   \  ___ \__  \   /     \ _/ __ \  /  ___/       **
**          |    |   \ / __ \_/ /_/ ||  Y Y  \ / __ \_|   |  \    \    \_\  \ / __ \_|  Y Y  \\  ___/  \___ \        **
**          |____|_  /(____  /\____ ||__|_|  /(____  /|___|  /     \______  /(____  /|__|_|  / \___  \/____  \       **
**                 \/      \/      \/      \/      \/      \/             \/      \/       \/      \/      \/        **
**                                                         2012                                                      **
**********************************************************************************************************************/

#include "QtOgreApplication.hpp"
#include </usr/include/OGRE/RenderSystems/GL/OgreGLPlugin.h>
#include "QOgreWidget.hpp" // must be included after the OGRE plugin

namespace rad
{
//---------------------------------------------------------------------------------------------------------------------
QtOgreApplication::QtOgreApplication() :
    ogreRoot_( new Ogre::Root() )
{
    // load the openGL plugin manually
    Ogre::GLPlugin* glPlugin = new Ogre::GLPlugin();
    Ogre::Root::getSingleton().installPlugin(glPlugin);

    // look for the openGL renderer in Ogre
    Ogre::RenderSystemList::const_iterator availableRendererIt = ogreRoot_->getAvailableRenderers().begin();
    while(availableRendererIt != ogreRoot_->getAvailableRenderers().end())
    {
        Ogre::String rName = (*availableRendererIt)->getName();
        if (rName == "OpenGL Rendering Subsystem")
        {
            break;
        }
        ++availableRendererIt;
    }

    if (availableRendererIt == ogreRoot_->getAvailableRenderers().end())
    {
        //throw std::runtime_error("We were unable to find the OpenGL renderer in ogre's list, cannot continue");
    }

    // use the OpenGL renderer in the root config
    renderSystem_ = *availableRendererIt;
    ogreRoot_->setRenderSystem( renderSystem_ );
    ogreRoot_->initialise(false);

    ogreWidget_ = new qt_ogre::QOgreWidget(ogreRoot_, this);

    sceneManager_ = ogreRoot_->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);
    camera_ = sceneManager_->createCamera( "QOgreWidget_Cam" );
    camera_->setPosition(1.0, 1.0, 200);

    ogreViewport_ = ogreWidget_->getEmbeddedOgreWindow()->addViewport( camera_ );
    ogreViewport_->setBackgroundColour( Ogre::ColourValue( 0, 1, 0 ) ); // make stuff green

    this->resize(640, 480);
    this->setWindowTitle("Most Basic Qt/Ogre Setup");

    this->setCentralWidget(ogreWidget_); // stick the ogre widget in the GUI

    this->show(); // give focus to our application and make it visible

}
//---------------------------------------------------------------------------------------------------------------------
QtOgreApplication::~QtOgreApplication()
{
    delete ogreRoot_;
    delete ogreWidget_;
}
//---------------------------------------------------------------------------------------------------------------------
}
