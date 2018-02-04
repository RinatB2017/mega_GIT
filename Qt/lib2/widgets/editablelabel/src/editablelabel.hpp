//--------------------------------------------------------------------------------
#include <QLineEdit>
//--------------------------------------------------------------------------------
class EditableLabel : public QLineEdit
{
    Q_OBJECT
public:
    EditableLabel( QWidget* p = 0 );
    QSize sizeHint(void) const;
    QSize minimumSizeHint(void) const;

private Q_SLOTS:
    void resizeByContents(void);
};
