/**********************************************************************************************************************
**         __________              ___                              ________                                         **
**         \______   \_____     __| _/ _____  _____     ____       /  _____/ _____     _____    ____    ______       **
**          |       _/\__  \   / __ | /     \ \__  \   /    \     /   \  ___ \__  \   /     \ _/ __ \  /  ___/       **
**          |    |   \ / __ \_/ /_/ ||  Y Y  \ / __ \_|   |  \    \    \_\  \ / __ \_|  Y Y  \\  ___/  \___ \        **
**          |____|_  /(____  /\____ ||__|_|  /(____  /|___|  /     \______  /(____  /|__|_|  / \___  \/____  \       **
**                 \/      \/      \/      \/      \/      \/             \/      \/       \/      \/      \/        **
**                                                         2011                                                      **
**********************************************************************************************************************/

#include <QResizeEvent>

#include <string>
#include <cassert>

#include "QOgreWidget.hpp"

namespace qt_ogre
{
//---------------------------------------------------------------------------------------------------------------------
QOgreWidget::QOgreWidget(Ogre::Root* ogreRoot, QWidget* parent) :
    QGLWidget(parent),
    ogreRoot_(ogreRoot),
    ogreRenderWindow_(NULL)
{
    if ( ! parent )
    {
        //throw std::runtime_error("Parent widget supplied was uninitialised!"); // interface requirement
    }

    std::string justXid = Ogre::StringConverter::toString((unsigned long)(this->parentWidget()->winId()));
    Ogre::NameValuePairList params;
    params["parentWindowHandle"] = justXid;

    ogreRenderWindow_ = ogreRoot_->createRenderWindow( "QOgreWidget_RenderWindow",
                                                       this->width(),
                                                       this->height(),
                                                       false,
                                                       &params );

    ogreRenderWindow_->setActive(true);
    ogreRenderWindow_->setVisible(true);

    WId ogreWinId = 0x0;
    ogreRenderWindow_->getCustomAttribute( "WINDOW", &ogreWinId );
    assert( ogreWinId ); // guaranteed to be valid due to the way it was created

    QWidget::create( ogreWinId );
    setAttribute( Qt::WA_OpaquePaintEvent ); // qt won't bother updating the area under/behind the render window

}

//---------------------------------------------------------------------------------------------------------------------
Ogre::RenderWindow* QOgreWidget::getEmbeddedOgreWindow()
{
    assert(ogreRenderWindow_); // guaranteed to be valid after construction
    return ogreRenderWindow_;

}
//---------------------------------------------------------------------------------------------------------------------
void QOgreWidget::paintEvent(QPaintEvent*)
{
    this->update();
}
//---------------------------------------------------------------------------------------------------------------------
void QOgreWidget::resizeEvent(QResizeEvent* rEvent)
{
    if (rEvent)
    {
        QWidget::resizeEvent( rEvent);
    }

    if (ogreRenderWindow_)
    {
        // since the underlying widget has already been updated we can source the resize values from there
        ogreRenderWindow_->reposition( x(), y() );
        ogreRenderWindow_->resize( width(), height() );
        ogreRenderWindow_->windowMovedOrResized();
    }

}
//---------------------------------------------------------------------------------------------------------------------
void QOgreWidget::update()
{
    QWidget::update();
    ogreRoot_->renderOneFrame();

}
//---------------------------------------------------------------------------------------------------------------------
}
