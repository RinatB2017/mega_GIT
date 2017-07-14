/*********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
//--------------------------------------------------------------------------------
#include <QSystemTrayIcon>
#include <QDesktopWidget>
#include <QStyleFactory>
#include <QResizeEvent>
#include <QApplication>
#include <QFontDialog>
#include <QTranslator>
#include <QToolButton>
#include <QPushButton>
#include <QMessageBox>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStatusBar>
#include <QSettings>
#include <QDateTime>
#include <QTextEdit>
#include <QToolBar>
#include <QMenuBar>
#include <QStyle>
#include <QTimer>
#include <QLabel>
#include <QMenu>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#ifdef Q_OS_LINUX
    #include <syslog.h>
#endif
#ifdef Q_OS_WIN
    #define LOG_EMERG       0       /* system is unusable */
    #define LOG_ALERT       1       /* action must be taken immediately */
    #define LOG_CRIT        2       /* critical conditions */
    #define LOG_ERR         3       /* error conditions */
    #define LOG_WARNING     4       /* warning conditions */
    #define LOG_NOTICE      5       /* normal but significant condition */
    #define LOG_INFO        6       /* informational */
    #define LOG_DEBUG       7       /* debug-level messages */
#endif
//--------------------------------------------------------------------------------
class LogBox;
class LogDock;
//--------------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &orgName,
                        const QString &appName,
                        const QString &appVersion,
                        QMainWindow* parent = 0);
    virtual ~MainWindow();

    void setCentralWidget(QWidget *widget);
    MainWindow *getThis(void);

#ifndef NO_MENU
    QMenuBar *get_menubar(void);
    QMenu *get_file_menu(void);
    QMenu *get_help_menu(void);
    QMenu *get_options_menu(void);
    QMenu *get_windows_menu(void);
#endif

    bool add_menu(int pos_x, QMenu *menu);
    bool add_action(QMenu *menu,
                    int pos_y,
                    QAction *action);
    bool add_separator(QMenu *menu,
                       int pos_y);

#ifndef NO_LOG
    void load_html(void);
    void save_html(void);
#endif

signals:
    void updateLanguage(void);

    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void message(const QString &);

    void syslog(int level,
                QString src,
                QString message);

public slots:
#ifndef NO_STATUSBAR
    void set_status1_text(const QString &data);
    void set_status2_text(const QString &data);
#endif

    void set_focus(const QString &);
#ifndef NO_LOG
    void set_log_font(void);
#endif

private slots:
    void log(const QString &data);
    void set_app_font(void);
    void setStyles(void);
    void closeOnExit(void);
    void about(void);
    void setMenuLanguage(void);
    void setToolBarLanguage(void);
    void help(void);
    void updateText(void);

    void load_setting(void);
    void save_setting(void);

#ifndef NO_TRAYICON
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
#endif
#ifndef NO_STYLETOOLBAR
    void setToolBarStyles(void);
#endif

private:

#ifndef ONLY_ENGLISH
    QTranslator *appTranslator = 0;
    QTranslator *sysTranslator = 0;
#endif
    QString orgName;
    QString appName;
    QString appVersion;
    QString style_name;

#ifndef NO_MENU
    QMenuBar *mainMenu = 0;

    QMenu *fileMenu = 0;
    QMenu *optionsMenu = 0;
#ifndef NO_LOG
    QMenu *windowsMenu = 0;
#endif
    QMenu *helpMenu = 0;

    QAction *m_Exit = 0;
    QAction *m_Help = 0;
    QAction *m_About = 0;
    QAction *m_Font = 0;
#ifndef NO_LOG
    QAction *m_Log_Font = 0;
#endif
    QAction *m_AskExit = 0;

    QMenu *styleMenu = 0;

#ifndef ONLY_ENGLISH
    QMenu *langMenu = 0;
#endif
#ifndef ONLY_ENGLISH
    QAction *m_Rus = 0;
    QAction *m_Eng = 0;
#endif
#endif

#ifndef NO_STATUSBAR
    QLabel *statusLabel1 = 0;
    QLabel *statusLabel2 = 0;
#endif

    QWidget *mainWidget = 0;

#ifndef NO_TRAYICON
    QSystemTrayIcon *trayIcon = 0;
    QMenu *trayIconMenu = 0;
#endif

    QAction *minimizeAction = 0;
    QAction *maximizeAction = 0;
    QAction *restoreAction = 0;
    QAction *quitAction = 0;

    bool flag_close = false;

    int app_left = 0;
    int app_top = 0;
    int app_width = 800;
    int app_height = 600;

    void load_main(void);
    void save_main(void);

    void init(void);

    void load_translations(void);
#ifndef NO_MENU
    void createMenus();
#endif
#ifndef NO_STATUSBAR
    void createStatusBar(void);
#endif
#ifndef NO_TRAYICON
    void createTrayIcon(void);
#endif

#ifndef NO_TOOLBAR
    QToolButton *btnExit = 0;
    QToolButton *btnFont = 0;
#ifndef ONLY_ENGLISH
    QToolButton *btnRus = 0;
    QToolButton *btnEng = 0;
#endif
    QToolButton *btnStyle = 0;
    QToolButton *btnHelp = 0;
    QToolButton *btnAbout = 0;
    void createToolBar(void);
#endif

#ifndef NO_STYLETOOLBAR
    QToolBar *toolBar = 0;
    void createStyleToolBar(void);
#endif

#ifndef NO_LOG
    LogDock *ld = 0;
    void createLog(void);
#endif

protected:
    virtual void closeEvent(QCloseEvent *event);
#ifndef NO_RESIZE
    virtual void resizeEvent (QResizeEvent * event);
#endif
};
//--------------------------------------------------------------------------------
#endif //MAINWINDOW_HPP
