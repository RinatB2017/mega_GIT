/**********************************************************************************************************************
**         __________              ___                              ________                                         **
**         \______   \_____     __| _/ _____  _____     ____       /  _____/ _____     _____    ____    ______       **
**          |       _/\__  \   / __ | /     \ \__  \   /    \     /   \  ___ \__  \   /     \ _/ __ \  /  ___/       **
**          |    |   \ / __ \_/ /_/ ||  Y Y  \ / __ \_|   |  \    \    \_\  \ / __ \_|  Y Y  \\  ___/  \___ \        **
**          |____|_  /(____  /\____ ||__|_|  /(____  /|___|  /     \______  /(____  /|__|_|  / \___  \/____  \       **
**                 \/      \/      \/      \/      \/      \/             \/      \/       \/      \/      \/        **
**                                                         2011                                                      **
**********************************************************************************************************************/

#ifndef QTOGRE_QOGREWIDGET_H
#define QTOGRE_QOGREWIDGET_H

#include "OGRE/Ogre.h"

#include <QtOpenGL/QGLWidget>

namespace qt_ogre
{
//*********************************************************************************************************************
/** Qt widget that wraps an embedded ogre render window
   *
   * This classes only responsibility is to initialise the ogre render window. The Ogre::Root object
   * to be used must be supplied by the user and once initialised the intended usage is to extract the
   * ogre render window and manipulate it directly to create the desired scene. The underlying widget
   * takes care of rendering and resizing when necessary.
   */
class QOgreWidget : public QGLWidget
{
    Q_OBJECT

public: // interface
    /** Constructor
     *
     * @param parent is required because ogre needs to set it as it's parent during initialisation
     *
     * @throws if parent is NULL
     *
     * @post Ogre3D renderwindow has been created
     */
    QOgreWidget(Ogre::Root* ogreRoot, QWidget* parent);

    Ogre::RenderWindow* getEmbeddedOgreWindow();

protected: // interface
    /*
     * @name Inherited from QWidget
     */
    //@{
    virtual void paintEvent(QPaintEvent*);
    virtual void resizeEvent(QResizeEvent* rEvent);
    virtual void update();
    //@}

private: // data
    Ogre::Root* ogreRoot_; // hold a pointer here so that we can trigger rendering
    Ogre::RenderWindow* ogreRenderWindow_;

}; // class

//*********************************************************************************************************************

} // namespace

#endif // QTOGRE_QOGREWIDGET_H
