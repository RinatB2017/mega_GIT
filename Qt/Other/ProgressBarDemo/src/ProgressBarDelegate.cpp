
#include <QApplication>

#include "ProgressBarDelegate.h"
#include "ProgressBarDemo.h"

static const int PROGRESS_BAR_HEIGHT_PX = 20;

ProgressBarDelegate::ProgressBarDelegate( QObject* parent ) : QStyledItemDelegate( parent )
{

}

void ProgressBarDelegate::paint(
        QPainter* painter,
        const QStyleOptionViewItem& option,
        const QModelIndex& index
        ) const {
    int progress = index.data().toInt();

    QStyleOptionProgressBar progressBarOption;
    QRect r = option.rect;
    r.setHeight( PROGRESS_BAR_HEIGHT_PX );
    r.moveCenter( option.rect.center() );
    progressBarOption.rect = r;
    progressBarOption.textAlignment = Qt::AlignCenter;
    progressBarOption.minimum = 0;
    progressBarOption.maximum = MAX_PROGRESS_VALUE;
    progressBarOption.progress = progress;
    progressBarOption.text = QString::number( progress ) + "%";
    progressBarOption.textVisible = true;

    QStyledItemDelegate::paint( painter, option, QModelIndex() );
    QApplication::style()->drawControl( QStyle::CE_ProgressBar, &progressBarOption, painter );
}
