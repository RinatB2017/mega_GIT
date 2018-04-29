#ifndef LOCK_BUTTON
#define LOCK_BUTTON
//--------------------------------------------------------------------------------
class LockButton : public QWidget
{
    Q_OBJECT

public:
    LockButton(QWidget *parent = 0) : QWidget(parent)
    {
        box = new QCheckBox(this);
        box->setToolTip("lock");

        btn = new QPushButton(this);
        btn->setText("Test");
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        connect(box,    SIGNAL(toggled(bool)),  btn,    SLOT(setDisabled(bool)));

        QHBoxLayout *hbox = new QHBoxLayout;
        hbox->setSpacing(0);
        hbox->setMargin(0);
        hbox->addWidget(box);
        hbox->addWidget(btn);
        hbox->setStretchFactor(box, 0);
        hbox->setStretchFactor(btn, 1);

        setLayout(hbox);
    }

    void setText(QString text)
    {
        btn->setText(text);
    }

private:
    QCheckBox *box   = 0;
    QPushButton *btn = 0;
};
//--------------------------------------------------------------------------------
#endif
