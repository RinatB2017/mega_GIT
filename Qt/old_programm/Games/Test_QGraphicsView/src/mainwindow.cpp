//--------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------
#include <QResizeEvent>
#include <QGridLayout>
#include <QEvent>
#include <QTimer>
//--------------------------------------------------------------------------------
#include "mainwindow.h"
//--------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    setLayout(new QGridLayout());
    graphicsView = new QGraphicsView();
    layout()->addWidget(graphicsView);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scene = new QGraphicsScene();

    for(int y=0; y<7; y++)
    {
        for(int x=0; x<7; x++)
        {
            QGraphicsEllipseItem *m_elipse = new QGraphicsEllipseItem(x*100, y*100, 100, 100);
            m_elipse->setPen(QPen(Qt::red));
            m_scene->addItem(m_elipse);

            QGraphicsTextItem *t_item = new QGraphicsTextItem(QString("%1:%2").arg(x).arg(y));
            t_item->setPos(x*100+35, y*100+37);
            m_scene->addItem(t_item);

        }
    }

    m_scene->setSceneRect(150, 150, 300, 300);

    graphicsView->setScene(m_scene);

    //graphicsView->viewport()->installEventFilter(this);

    min_x = 0;
    max_x = 400;
    min_y = 0;
    max_y = 400;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(5);

    setFixedSize(sizeHint());

    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}
//--------------------------------------------------------------------------------
MainWindow::~MainWindow()
{

}
//--------------------------------------------------------------------------------
void MainWindow::update(void)
{
#if 0
    x += delta_x;
    y += delta_y;

    if(x < 1) delta_x *= -1;
    if(y < 1) delta_y *= -1;

    if(x > 400) delta_x *= -1;
    if(y > 400) delta_y *= -1;

    m_scene->setSceneRect((int)x, (int)y, 300, 300);
#else
    //qreal old_pos_x = pos_x;
    //qreal old_pos_y = pos_y;

    if(flag_up)     pos_y -= delta;
    if(flag_down)   pos_y += delta;
    if(flag_left)   pos_x -= delta;
    if(flag_right)  pos_x += delta;

    if(pos_x < min_x)   pos_x = min_x;
    if(pos_x > max_x)   pos_x = max_x;
    if(pos_y < min_y)   pos_y = min_y;
    if(pos_y > max_y)   pos_y = max_y;

    m_scene->setSceneRect(pos_x, pos_y, 300, 300);
#endif
}
//--------------------------------------------------------------------------------
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:    flag_up    = true;  break;
    case Qt::Key_Down:  flag_down  = true;  break;
    case Qt::Key_Left:  flag_left  = true;  break;
    case Qt::Key_Right: flag_right = true;  break;
    default:
        QWidget::keyPressEvent(event);
        break;
    }
}
//--------------------------------------------------------------------------------
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:    flag_up    = false;  break;
    case Qt::Key_Down:  flag_down  = false;  break;
    case Qt::Key_Left:  flag_left  = false;  break;
    case Qt::Key_Right: flag_right = false;  break;
    default:
        QWidget::keyPressEvent(event);
        break;
    }
}
//--------------------------------------------------------------------------------
bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    if(event->type() == QEvent::Resize )
    {
        //QResizeEvent *res = reinterpret_cast<QResizeEvent*>(event);
        //m_elipse->setRect(0, 0, res->size().width(), res->size().height());
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
