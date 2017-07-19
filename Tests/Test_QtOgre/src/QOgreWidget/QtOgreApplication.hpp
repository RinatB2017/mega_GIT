/**********************************************************************************************************************
**         __________              ___                              ________                                         **
**         \______   \_____     __| _/ _____  _____     ____       /  _____/ _____     _____    ____    ______       **
**          |       _/\__  \   / __ | /     \ \__  \   /    \     /   \  ___ \__  \   /     \ _/ __ \  /  ___/       **
**          |    |   \ / __ \_/ /_/ ||  Y Y  \ / __ \_|   |  \    \    \_\  \ / __ \_|  Y Y  \\  ___/  \___ \        **
**          |____|_  /(____  /\____ ||__|_|  /(____  /|___|  /     \______  /(____  /|__|_|  / \___  \/____  \       **
**                 \/      \/      \/      \/      \/      \/             \/      \/       \/      \/      \/        **
**                                                         2012                                                      **
**********************************************************************************************************************/

#ifndef GAMETESTBED_QTOGREAPPLICATION_HPP
#define GAMETESTBED_QTOGREAPPLICATION_HPP

#include "OGRE/Ogre.h"

#include <QMainWindow>

namespace qt_ogre
{
  class QOgreWidget;  // use forward declare here because there is some stupid GL header include order problem,
                      // it means that the Ogre GL RenderSystem header must be included before QOgreWidget.hpp
}

namespace rad
{
//*********************************************************************************************************************
  class QtOgreApplication : public QMainWindow
  {
    Q_OBJECT

  public: // interface
    QtOgreApplication();
    ~QtOgreApplication();

  private: // data
    qt_ogre::QOgreWidget* ogreWidget_;

    Ogre::Root* ogreRoot_;
    Ogre::Viewport* ogreViewport_;
    Ogre::RenderSystem* renderSystem_;
    Ogre::SceneManager* sceneManager_;
    Ogre::Camera* camera_;

  }; // class

//*********************************************************************************************************************

} // namespace

#endif // GAMETESTBED_QTOGREAPPLICATION_HPP
