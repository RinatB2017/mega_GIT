#ifndef GLASS_HPP
#define GLASS_HPP
//--------------------------------------------------------------------------------
#include <QString>
#include <QObject>
#include <QMovie>
#include <QColor>
#include <QLabel>
//--------------------------------------------------------------------------------
class Glass: public QObject
{
    Q_OBJECT
public:
    Glass();
    virtual ~Glass();

    virtual void install(QWidget* widget);
    virtual void remove();

    void enableColor(const QColor& color = QColor(111, 111, 100));
    void disableColor();

    void enableOpacity(qreal opacity = 0.5);
    void disableOpacity();

    void enableInfoBlock(QMovie* movie = 0, const QString& text = QString::null);
    void disableInfoBlock();

    void enableAnimationBlock(QMovie* movie = 0);
    void disableAnimationBlock();

    void enableInfoTextBlock(const QString& text = QString::null);
    void disableInfoTextBlock();
    QLabel& getInfoTextBlock();
    void setMovie(QMovie* movie);

private:
    void showInfoTextBlock(const QString& text = QString::null);
    void showAnimationBlock(QMovie* movie = 0);
    void infoBlockPositioning();

    QLabel* glass;
    QLabel* animationContainer;
    QMovie* movie;
    QMovie* defaultMovie;
    QLabel* infoTextContaiter;
    QColor glassColor;
    int countPerSecond;

    bool infoTextBlockEnabled;
    bool animationBlockEnabled;

protected:
    bool eventFilter(QObject* object, QEvent* event);
};
//--------------------------------------------------------------------------------
#endif
