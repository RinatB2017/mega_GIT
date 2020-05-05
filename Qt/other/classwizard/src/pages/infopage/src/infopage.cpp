
#include "infopage.hpp"

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/watermark1.png"));

    label = new QLabel(tr("This wizard will generate a skeleton C++ class "
                          "definition, including a few functions. You simply "
                          "need to specify the class name and set a few "
                          "options to produce a header file and an "
                          "implementation file for your new C++ class."));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}
