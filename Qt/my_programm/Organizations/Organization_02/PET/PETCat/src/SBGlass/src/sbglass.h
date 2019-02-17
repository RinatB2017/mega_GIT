#ifndef SBGLASS_H
#define SBGLASS_H

/*
****************************************
[5/22/2013 Bepec]Специально для prog.org.ru
Часть кода взята из статьи "Qt. Накрываем виджеты стеклом" (http://www.quizful.net/post/glass-qt-component)
****************************************
*/

#include <QObject>
#include <QLabel>
#include <QMovie>
#include <QEvent>
#include <QBrush>
#include <QGraphicsOpacityEffect>

#ifdef QT_DEBUG
#   include <QDebug>
#endif


class SBGlass : public QObject
{
	Q_OBJECT

public:
    SBGlass(QObject *parent = nullptr);
	~SBGlass();
	virtual void install(QWidget* w);
	virtual void remove();

	void enableColor(const QColor& color = QColor(111, 111, 100));
	void disableColor();

	void enableOpacity(qreal opacity = 0.5);
	void disableOpacity();

    void enableInfoBlock(QMovie* movie = nullptr, const QString& text = QString::null);
	void disableInfoBlock();

    void enableAnimationBlock(QMovie* movie = nullptr);
	void disableAnimationBlock();

	void enableInfoTextBlock(const QString& text = QString::null);
	void disableInfoTextBlock();
	QLabel& getInfoTextBlock();
	void setMovie(QMovie* movie);

protected:
	bool eventFilter(QObject* object, QEvent* event);

private:
	void showInfoTextBlock(const QString& text = QString::null);
    void showAnimationBlock(QMovie* movie = nullptr);
	void infoBlockPositioning();

private:
	QLabel* wGlass_;
	QLabel* wAnimationContainer_;
	QLabel* wInfoTextContaiter_;
	QMovie* movie_;
	QMovie* defaultMovie_;
	QColor glassColor_;

    bool flagInfoTextBlockEnabled_;
    bool flagAnimationBlockEnabled_;
};

#endif // SBGLASS_H
