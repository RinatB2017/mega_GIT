
#include <QVBoxLayout>
#include <QGridLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QWidget>
#include <QWizard>
#include <QLabel>

class ClassInfoPage : public QWizardPage
{
    Q_OBJECT

public:
    ClassInfoPage(QWidget *parent = nullptr);

private:
    QLabel *classNameLabel;
    QLabel *baseClassLabel;
    QLineEdit *classNameLineEdit;
    QLineEdit *baseClassLineEdit;
    QCheckBox *qobjectMacroCheckBox;
    QGroupBox *groupBox;
    QRadioButton *qobjectCtorRadioButton;
    QRadioButton *qwidgetCtorRadioButton;
    QRadioButton *defaultCtorRadioButton;
    QCheckBox *copyCtorCheckBox;
};
