
#include <QApplication>

#include "WindowEventListener.h"
#include "QtOgre.h"
#include "QOgreWindow.h"

//class MyWindowEventListener : public Ogre::WindowEventListener
//{
//    public:
//        MyWindowEventListener()
//        {
//            //mWindow = window;
//            //mInputHandler = 0;
//            Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
//        }

//        ~MyWindowEventListener()
//        {
//            Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
//        }

//        void setInputHandler(InputHandler*)
//        {
//            //mInputHandler = input;
//        }

//        virtual void windowResized(Ogre::RenderWindow* window)
//        {
//            if (window != mWindow)
//            {
//                return;
//            }
//            if (mInputHandler)
//            {
//                unsigned int width;
//                unsigned int height;
//                unsigned int colorDepth;
//                int left;
//                int top;
//                window->getMetrics(width, height, colorDepth, left, top);
//                mInputHandler->setWindowSize(width, height);
//            }
//        }

//        virtual void windowClosed(Ogre::RenderWindow* window)
//        {
//            if (window != mWindow)
//            {
//                return;
//            }
//            //if (mInputHandler)
//            //{
//            //    mInputHandler->notifyWindowClosed();
//            //}
//        }
//    private:
//        //InputHandler* mInputHandler;
//        Ogre::RenderWindow* mWindow;
//};

class MyWindowEventListener : public Ogre::WindowEventListener
{
public:
    void onGraphics() {}
    void onAction(const int &) {}
    void onKeyPressed(const uchar &) {}
    void onKeyReleased(const uchar &) {}
    void onMouseMoved(const sint &, const sint &) {}
    void onSetup(Ogre::RenderWindow* ) {}
};

int main(int argc, char** argv)
{
		QApplication app(argc, argv);

        WindowEventListener *listener = new MyWindowEventListener();
        QtOgre::QOgreWindow window(listener);
        //QtOgre::QOgreWindow window(0);
		window.show();

		return app.exec();
}
