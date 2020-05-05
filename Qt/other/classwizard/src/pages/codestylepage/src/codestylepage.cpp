
#include "codestylepage.hpp"

CodeStylePage::CodeStylePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Code Style Options"));
    setSubTitle(tr("Choose the formatting of the generated code."));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/images/logo2.png"));

    commentCheckBox = new QCheckBox(tr("&Start generated files with a "
                                       "comment"));
    commentCheckBox->setChecked(true);

    protectCheckBox = new QCheckBox(tr("&Protect header file against multiple "
                                       "inclusions"));
    protectCheckBox->setChecked(true);

    macroNameLabel = new QLabel(tr("&Macro name:"));
    macroNameLineEdit = new QLineEdit;
    macroNameLabel->setBuddy(macroNameLineEdit);

    includeBaseCheckBox = new QCheckBox(tr("&Include base class definition"));
    baseIncludeLabel = new QLabel(tr("Base class include:"));
    baseIncludeLineEdit = new QLineEdit;
    baseIncludeLabel->setBuddy(baseIncludeLineEdit);

    connect(protectCheckBox, &QAbstractButton::toggled,
            macroNameLabel, &QWidget::setEnabled);
    connect(protectCheckBox, &QAbstractButton::toggled,
            macroNameLineEdit, &QWidget::setEnabled);
    connect(includeBaseCheckBox, &QAbstractButton::toggled,
            baseIncludeLabel, &QWidget::setEnabled);
    connect(includeBaseCheckBox, &QAbstractButton::toggled,
            baseIncludeLineEdit, &QWidget::setEnabled);

    registerField("comment", commentCheckBox);
    registerField("protect", protectCheckBox);
    registerField("macroName", macroNameLineEdit);
    registerField("includeBase", includeBaseCheckBox);
    registerField("baseInclude", baseIncludeLineEdit);

    QGridLayout *layout = new QGridLayout;
    layout->setColumnMinimumWidth(0, 20);
    layout->addWidget(commentCheckBox, 0, 0, 1, 3);
    layout->addWidget(protectCheckBox, 1, 0, 1, 3);
    layout->addWidget(macroNameLabel, 2, 1);
    layout->addWidget(macroNameLineEdit, 2, 2);
    layout->addWidget(includeBaseCheckBox, 3, 0, 1, 3);
    layout->addWidget(baseIncludeLabel, 4, 1);
    layout->addWidget(baseIncludeLineEdit, 4, 2);

    setLayout(layout);
}

void CodeStylePage::initializePage()
{
    QString className = field("className").toString();
    macroNameLineEdit->setText(className.toUpper() + "_H");

    QString baseClass = field("baseClass").toString();

    includeBaseCheckBox->setChecked(!baseClass.isEmpty());
    includeBaseCheckBox->setEnabled(!baseClass.isEmpty());
    baseIncludeLabel->setEnabled(!baseClass.isEmpty());
    baseIncludeLineEdit->setEnabled(!baseClass.isEmpty());

    QRegularExpression rx("Q[A-Z].*");
    if (baseClass.isEmpty())
    {
        baseIncludeLineEdit->clear();
    } else if (rx.match(baseClass).hasMatch())
    {
        baseIncludeLineEdit->setText('<' + baseClass + '>');
    }
    else
    {
        baseIncludeLineEdit->setText('"' + baseClass.toLower() + ".hpp\"");
    }
}
