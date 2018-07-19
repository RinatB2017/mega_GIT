#ifndef TEST_QGLWIDGET_HPP
#define TEST_QGLWIDGET_HPP
//--------------------------------------------------------------------------------
#include <QtWidgets>
//--------------------------------------------------------------------------------
#include <QGLWidget>
#include <GL/glu.h>
//--------------------------------------------------------------------------------
class Test_QGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    Test_QGLWidget(QWidget *parent = 0);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);

private slots:
    void choice_test(void);
    bool test_0(void);
    bool test_1(void);
    bool test_2(void);
    bool test_3(void);
    bool test_4(void);
    bool test_5(void);

private:
    enum {
        ID_TEST_0 = 1000,
        ID_TEST_1,
        ID_TEST_2,
        ID_TEST_3,
        ID_TEST_4,
        ID_TEST_5,
        ID_TEST_6
    };
    typedef struct CMD
    {
        int cmd;
        QString cmd_text;
        bool (Test_QGLWidget::*func)(void);
    } CMD_t;

    QComboBox *cb_test = 0;
    QList<CMD> commands;

    void createTestBar(void);
    QToolButton *add_button(QToolBar *tool_bar,
                            QToolButton *tool_button,
                            QIcon icon,
                            const QString &text,
                            const QString &tool_tip);

    void draw();
    void draw_sphere();
    void draw_cylinder();

    int faceAtPosition(const QPoint &pos);

    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;
    QColor faceColors[4];
    QPoint lastPos;

    GLUquadricObj *cylinder;
    GLUquadricObj *sphere;

    GLfloat x = 0;
    GLfloat y = 0;
    GLfloat z = 0;
};
//--------------------------------------------------------------------------------
#endif
