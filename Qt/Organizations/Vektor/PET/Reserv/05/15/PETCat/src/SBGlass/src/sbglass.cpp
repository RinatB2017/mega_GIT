#include "sbglass.h"

SBGlass::SBGlass(QObject *parent)
    : QObject(parent)
    , movie_(0)
    , defaultMovie_(0)
{
    flagAnimationBlockEnabled_  = true;
    flagInfoTextBlockEnabled_   = true;

    wGlass_ = new QLabel();
    wAnimationContainer_ = new QLabel();
    wInfoTextContaiter_ = new QLabel();

    // Загрузим анимацию по умолчанию
    defaultMovie_ = new QMovie(":/SBGlass/loading.gif", QByteArray(), wAnimationContainer_);
    //enableInfoBlock(defaultMovie_, "");
}

SBGlass::~SBGlass()
{
    wGlass_->deleteLater();
    wAnimationContainer_->deleteLater();
    wInfoTextContaiter_->deleteLater();
    defaultMovie_->deleteLater();
}

void SBGlass::install(QWidget* widget)
{
    // Для начала удалим его с предыдущего виджета
    remove();

    // Установим стекло поверх виджета
    wGlass_->setParent(widget);

    // Начнем отлавливать все события, установив фильтр
    widget->installEventFilter(this);

    // Покажем стекло и информационный блок, если это необходимо
    wGlass_->show();
    if (flagInfoTextBlockEnabled_)
        showInfoTextBlock();

    if (flagAnimationBlockEnabled_)
        showAnimationBlock();

    // Пошлем событие сами себе, чтобы стекло и информационный блок правильно
    // разместились на виджете
    QEvent event(QEvent::Resize);
    eventFilter(0, &event);
}

void SBGlass::remove()
{
    // Если стекло было установлено, то удаляем его
    if (wGlass_->parentWidget() != 0)
    {
        // Перестаем отлавливать события на низлежащем виджете
        wGlass_->parentWidget()->removeEventFilter(this);

        // Скрываем все компоненты стекла
        wGlass_->hide();
        wGlass_->setParent(0);
        wAnimationContainer_->hide();
        wInfoTextContaiter_->hide();
        wAnimationContainer_->setParent(0);
        wInfoTextContaiter_->setParent(0);
    }
}

// Реализуем фильтр событий на виджете. Назначение этого фильтра - не
// допустить работу с виджетом под стеклом и обеспечить актуальные размеры и перерисовку стекла
bool SBGlass::eventFilter(QObject* /* object */, QEvent* event) 
{
    // Если показывается виджет или изменились его размеры, изменяем размеры
    // стекла и позиционируем информационный блок
    if ((event->type() == QEvent::Show) || (event->type() == QEvent::Resize))
    {
        wGlass_->resize(wGlass_->parentWidget()->size());
        wGlass_->move(0, 0);
        infoBlockPositioning();
        return true;
    }

    if (event->type() ==  QEvent::Shortcut || event->type() ==  QEvent::ShortcutOverride)
    {

    }
    // Всегда отбираем фокус, чтобы запретить работу с виджетом под стеклом
    wGlass_->setFocus();
    //	event->	ignore ();
    // убираем возможность вызова сочетаний клавиш
    event->accept();
    return false;
}

// Реализуем позиционирование информационного блока. Он будет размещаться по
// центру стекла. Под анимацией будем отображать пояснительный текст.
void SBGlass::infoBlockPositioning() 
{
    if (wAnimationContainer_->isVisible() && wInfoTextContaiter_->isVisible())
    {
        wAnimationContainer_->move((wGlass_->width() - wAnimationContainer_->width()) / 2,
                                   wGlass_->height() / 2 - wAnimationContainer_->height());
        wInfoTextContaiter_->move((wGlass_->width() - wInfoTextContaiter_->width()) / 2,
                                  wGlass_->height() / 2 + wInfoTextContaiter_->height());
    }
    else
    {
        if (wAnimationContainer_->isVisible())
            wAnimationContainer_->move((wGlass_->width() - wAnimationContainer_->width()) / 2,
                                       (wGlass_->height() - wAnimationContainer_->height()) / 2);

        if (wInfoTextContaiter_->isVisible())
            wInfoTextContaiter_->move((wGlass_->width() - wInfoTextContaiter_->width()) / 2,
                                      (wGlass_->height() - wInfoTextContaiter_->height()) / 2);
    }
}

// Реализуем методы показа информационного блока
void SBGlass::showInfoTextBlock(const QString& text) {
    // Устанавливаем пояснительный текст поверх стекла и накрытого виджета. Здесь
    // используется особенность размещения графических компонентов Qt: последний
    // добавленный графический компонент будет выше всех остальных
    wInfoTextContaiter_->setParent(wGlass_->parentWidget());
    if (!text.isNull())
        wInfoTextContaiter_->setText(text);

    wInfoTextContaiter_->show();
    wInfoTextContaiter_->adjustSize();
    // Корректируем позицию
    infoBlockPositioning();
}

void SBGlass::showAnimationBlock(QMovie* movie) {
    // Устанавливаем анимационный контейнер поверх стекла и накрытого виджета
    wAnimationContainer_->setParent(wGlass_->parentWidget());
    if (movie != 0)
        setMovie(movie);
    wAnimationContainer_->show();
    infoBlockPositioning();
}

void SBGlass::enableColor(const QColor& color )
{
    glassColor_ = color;
    wGlass_->setStyleSheet(QString("background-color: %1;").arg(color.name()));
}

void SBGlass::disableColor()
{
    glassColor_ = QColor();
    wGlass_->setStyleSheet("");
}

void SBGlass::enableOpacity(qreal opacity )
{
    QGraphicsOpacityEffect * tmpEffect = new QGraphicsOpacityEffect;
    tmpEffect->setOpacity(opacity);
    wGlass_->setGraphicsEffect(tmpEffect);
}

void SBGlass::disableOpacity()
{
    QGraphicsOpacityEffect off;
    wGlass_->setGraphicsEffect(&off);
}

void SBGlass::enableInfoBlock(QMovie* movie, const QString& text)
{
    //setMovie(movie);
    enableAnimationBlock(movie);
    enableInfoTextBlock(text);
    //wInfoTextContaiter_->show();
    //wAnimationContainer_->show();
    infoBlockPositioning();
}

void SBGlass::disableInfoBlock()
{
    wAnimationContainer_->hide();
    wInfoTextContaiter_->hide();
}	

void SBGlass::enableAnimationBlock(QMovie* movie )
{
    setMovie(movie);
    wAnimationContainer_->show();
    infoBlockPositioning();
}

void SBGlass::disableAnimationBlock()
{
    wAnimationContainer_->hide();
}

void SBGlass::enableInfoTextBlock(const QString& text )
{
    wInfoTextContaiter_->setText(text);
    wInfoTextContaiter_->adjustSize();
    wInfoTextContaiter_->show();
    infoBlockPositioning();
}

void SBGlass::disableInfoTextBlock()
{
    wInfoTextContaiter_->hide();
}

QLabel& SBGlass::getInfoTextBlock()
{
    return *wInfoTextContaiter_;
}

void SBGlass::setMovie(QMovie* movie)
{
    delete movie_;
    movie_ = movie;
    if (movie_ != 0)
    {
        wAnimationContainer_->setMovie(movie_);
        movie_->jumpToFrame(0);
        wAnimationContainer_->resize(movie_->currentPixmap().size());

        wAnimationContainer_->show();
        movie_->start();
    }
    else
    {
        QMovie off;
        wAnimationContainer_->setMovie(&off);
    }

    infoBlockPositioning();
}
