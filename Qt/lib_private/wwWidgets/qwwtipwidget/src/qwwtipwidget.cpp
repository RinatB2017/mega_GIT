//
// C++ Implementation: QwwTipWidget
//
// Description:
//
//
// Author: Witold Wysota <wwwidgets@wysota.eu.org>, (C) 2007-2010
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "qwwtipwidget.h"
#ifndef WW_NO_TIPWIDGET

#include <QHBoxLayout>

#include <QTextBrowser>
#include <QStringListModel>
#include <QEvent>
#include <QPixmap>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include "wwglobal_p.h"

/**
 *  \internal
 *
 */
class QwwTipWidgetPrivate : public QwwPrivate {
public:
    QwwTipWidgetPrivate(QwwTipWidget *par) : QwwPrivate(par) {

    }
    void initUi();
    void retranslateUi();
    QwwTipWidget *pub;
    QLabel *label;
    QAbstractItemModel *m_model;
	QWidget *m_headerWidget;
    QPersistentModelIndex m_currentTip;
	QPushButton *m_next, *m_prev, *m_close;
    QTextBrowser *m_browser;
	QCheckBox *m_check;
    QIcon defaultIcon;
    QSize iconSize;
    void showTip() {
        m_browser->setHtml(m_currentTip.data(Qt::DisplayRole).toString());
        QVariant v = m_currentTip.data(Qt::DisplayRole);
        if (v.type()==QVariant::Icon) {
            QIcon ic = qvariant_cast<QIcon>(v);
            label->setPixmap(ic.pixmap(iconSize));
        } else if (v.type()==QVariant::Pixmap) {
            label->setPixmap(qvariant_cast<QPixmap>(v));
        } else if (v.type()==QVariant::Image) {
            label->setPixmap(QPixmap::fromImage(qvariant_cast<QImage>(v)));
        } else
            label->setPixmap(defaultIcon.pixmap(iconSize));
    }
private:
    WW_DECLARE_PUBLIC(QwwTipWidget);
};


/*!
 *  \class QwwTipWidget
 *  \inmodule wwWidgets
 *  \brief The QwwTipWidget class provides a widget that displays application tips.
 *
 *  The widget can be embedded into a dialog and connected to its signals
 *  to provide a quick way to show startup tips for the user.
 *
 *  \image qwwtipwidget.png QwwTipWidget
 *
 *  Tips displayed are adjustable using the \a tips property. The widget can contain
 *  a checkbox that asks whether the user wants to see the tips again next time
 *  the application is started. This checkbox doesn't implement any logic, if you want
 *  to have that behavior in your application, you have to implement it yourself using
 *  some persistent data storage like QSettings.
 *
 *  Tips are displayed in rich text, so they can contain formatting, images and other
 *  elements that can be rendered using Scribe.
 */
/*!
 *  \property QwwTipWidget::checkVisible
 *
 *  This property holds whether the "show tips on startup" checkbox is visible.
 */
/*!
 *  \property QwwTipWidget::closeVisible
 *
 *  This property holds whether the close button is visible.
 */
/*!
 *  \property QwwTipWidget::tips
 *
 *  This property holds a list of tips associated with the widget.
 */
/*!
 *  \property QwwTipWidget::currentTip
 *
 *  This property holds index of the currently shown tip.
 */
/*!
 * \property QwwTipWidget::tipsEnabled
 * \brief    This property holds whether showing tips is enabled.
 */
/*!
 * \fn      void QwwTipWidget::tipChanged(int index)
 * \brief   This signal is emitted whenever the currently shown tip changes to \a index
 */

void QwwTipWidgetPrivate::initUi() {
    Q_Q(QwwTipWidget);
    QHBoxLayout *l = new QHBoxLayout(q);
    label = new QLabel;
    iconSize = QSize(128,128);
    m_browser = new QTextBrowser;
    m_browser->setOpenExternalLinks(true);

    l->addWidget(label);
    l->addWidget(m_browser);
    q->setBackgroundRole(QPalette::Base);
    q->setFrameShape(m_browser->frameShape());
    q->setFrameShadow(m_browser->frameShadow());
    m_browser->setFrameShape(QFrame::NoFrame);
    QPalette pal = m_browser->palette();
    q->setPalette(pal);
    pal.setColor(QPalette::Base, Qt::transparent);
    m_browser->setPalette(pal);
    q->setAutoFillBackground(true);
    m_browser->setAutoFillBackground(false);
#if QT_VERSION >= 0x040200
    m_browser->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
#endif
#if QT_VERSION >= 0x040300
    l->setContentsMargins(0, 0, 0, 0);
#endif
    retranslateUi();
}


void QwwTipWidgetPrivate::retranslateUi() {
//    Q_Q(QwwTipWidget);

}

/*!
 * \brief Constructs a tip widget with \a list of tips and a given \a parent
 */
QwwTipWidget::QwwTipWidget(const QStringList & list, QWidget * parent) : QFrame(parent), QwwPrivatable(new QwwTipWidgetPrivate(this)) {
    Q_D(QwwTipWidget);
    d->m_model = new QStringListModel(list, this);
    d->m_currentTip = d->m_model->index(0,0);
    d->initUi();
    d->showTip();
}


int QwwTipWidget::currentTip() const {
    Q_D(const QwwTipWidget);
    return d->m_currentTip.row();
}


/*!
 *  \brief sets \a hw as a header of the widget
 *
 */
void QwwTipWidget::setHeaderWidget(QWidget *hw) {
    Q_D(QwwTipWidget);
    QVBoxLayout *l = dynamic_cast<QVBoxLayout*>(layout());
    if (!l) return;
    delete d->m_headerWidget;
    l->insertWidget(0, hw);
    d->m_headerWidget = hw;
}

/*!
 *  \brief returns a pointer to the header widget
 *
 */
QWidget * QwwTipWidget::headerWidget() const {
    Q_D(const QwwTipWidget);
    return d->m_headerWidget;
}

/*!
 *  \brief returns a pointer to the "next" button
 *
 */
const QPushButton * QwwTipWidget::nextButton() const {
    Q_D(const QwwTipWidget);
    return d->m_next;
}

/*!
 *  \brief returns a pointer to the "prev" button
 *
 */
const QPushButton * QwwTipWidget::prevButton() const {
    Q_D(const QwwTipWidget);
    return d->m_prev;
}

/*!
 *  \brief returns a pointer to the "close" button
 *
 */
const QPushButton * QwwTipWidget::closeButton() const {
    Q_D(const QwwTipWidget);
    return d->m_close;
}

/*!
 *  \brief returns a pointer to the tip canvas
 *
 */
const QTextBrowser * QwwTipWidget::tipCanvas() const {
    Q_D(const QwwTipWidget);
    return d->m_browser;
}

/*!
 *  Returns whether the "show tips on startup" checkbox is visible
 *  \sa setCheckVisible
 */
bool QwwTipWidget::checkIsVisible() const {
    Q_D(const QwwTipWidget);
    return d->m_check->isVisible();
}

/*!
 *  Returns whether the close button is visible
 *  \sa closeButton, setCloseVisible
 */
bool QwwTipWidget::closeIsVisible() const {
    Q_D(const QwwTipWidget);
    return d->m_close->isVisible();
}


/*!
 *  This slot changes the current tip to the next one. In case the currently
 *  displayed tip is the last one, function will make the first tip current.
 */
void QwwTipWidget::nextTip() {
    Q_D(QwwTipWidget);
    int r = currentTip();
    int c = d->m_model->rowCount();
    r = r==c-1 ? 0 : r+1;
    setCurrentTip(r);
}

/*!
 *  This slot changes the current tip to the previous one. In case the currently
 *  displayed tip is the first one, function will make the last tip current.
 */
void QwwTipWidget::prevTip() {
    Q_D(QwwTipWidget);
    int r = currentTip();
    int c = d->m_model->rowCount();
    r = r==0 ? c-1 : r-1;
    setCurrentTip(r);
}

void QwwTipWidget::setTipsEnabled(bool v) {
	Q_D(QwwTipWidget);
	d->m_check->setChecked(v);
}

/*!
 *  Changes the visibility of the "show tips on startup" checkbox to \a v
 *  \sa checkIsVisible, setCheckHidden
 */
void QwwTipWidget::setCheckVisible(bool v) {
    Q_D(QwwTipWidget);
    d->m_check->setVisible(v);
}

/*!
 *  Changes the visibility of the close button to \a v
 *  \sa closeIsVisible, setCloseHidden, closeButton
 */
void QwwTipWidget::setCloseVisible(bool v){
    Q_D(QwwTipWidget);
    d->m_close->setVisible(v);
}

/*!
 * Convenience function. Equivalent to setCloseVisible(!\a v).
 */
void QwwTipWidget::setCloseHidden(bool v){
    Q_D(QwwTipWidget);
    d->m_close->setVisible(!v);
}

/*!
 * Convenience function. Equivalent to setCheckVisible(!\a v).
 */
void QwwTipWidget::setCheckHidden(bool v) {
    Q_D(QwwTipWidget);
    d->m_check->setVisible(!v);
}

QFrame::Shape QwwTipWidget::canvasFrameShape() const {
    Q_D(const QwwTipWidget);
    return d->m_browser->frameShape();
}

/*!
 *
 */
void QwwTipWidget::setCanvasFrameShape(QFrame::Shape s) {
    Q_D(QwwTipWidget);
	d->m_browser->setFrameShape(s);
}

bool QwwTipWidget::tipsEnabled() const
{
	Q_D(const QwwTipWidget);
	return d->m_check->isChecked();
}

/*!
 *
 */
void QwwTipWidget::setTips(const QStringList &slist) {
    Q_D(QwwTipWidget);
    delete d->m_model;
    d->m_model = new QStringListModel(slist, this);
    setCurrentTip(0);
}

/*!
 *
 */
const QStringList QwwTipWidget::tips() const {
    Q_D(const QwwTipWidget);
    return ((QStringListModel*)(d->m_model))->stringList();
}

/*!
 *
 */
void QwwTipWidget::setCurrentTip(int v) {
    Q_D(QwwTipWidget);
    if (v>=d->m_model->rowCount()) v= d->m_model->rowCount()-1;
    if (v<0) v = 0;
    if (v==currentTip()) return;
    d->m_currentTip = d->m_model->index(v,0);
    d->showTip();
    emit tipChanged(v);
}

/*!
 * \internal
 */
void QwwTipWidget::changeEvent(QEvent *e) {
    if (e->type()==QEvent::LanguageChange) {
        Q_D(QwwTipWidget);
        d->retranslateUi();
    }
    QWidget::changeEvent(e);
}

void QwwTipWidget::setDefaultIcon(const QIcon &i) {
    Q_D(QwwTipWidget);
    d->defaultIcon = i;
    d->showTip();
}
QIcon QwwTipWidget::defaultIcon() const {
    Q_D(const QwwTipWidget);
    return d->defaultIcon;
}

QSize QwwTipWidget::iconSize() const {
    Q_D(const QwwTipWidget);
    return d->iconSize;
}

void QwwTipWidget::setIconSize(const QSize &s) {
    Q_D(QwwTipWidget);
    d->iconSize = s;
    d->showTip();
}

Qt::Alignment QwwTipWidget::iconAlignment() const {
    Q_D(const QwwTipWidget);
    return d->label->alignment() & Qt::AlignVertical_Mask;
}
void QwwTipWidget::setIconAlignment(Qt::Alignment a) {
    Q_D(QwwTipWidget);
    d->label->setAlignment(a);
}

#include "moc_qwwtipwidget.cpp"
#endif
