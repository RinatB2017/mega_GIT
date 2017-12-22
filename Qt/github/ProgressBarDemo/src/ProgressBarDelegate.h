#ifndef PROGRESSBAR_DELEGATE
#define PROGRESSBAR_DELEGATE

#include <QStyledItemDelegate>

class ProgressBarDelegate : public QStyledItemDelegate
{
public:
    ProgressBarDelegate( QObject* parent = 0 );
    void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
};

#endif
