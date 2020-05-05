
#include <QRegularExpression>
#include <QWizardPage>
#include <QGridLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>

class CodeStylePage : public QWizardPage
{
    Q_OBJECT

public:
    CodeStylePage(QWidget *parent = nullptr);

protected:
    void initializePage() override;

private:
    QCheckBox *commentCheckBox;
    QCheckBox *protectCheckBox;
    QCheckBox *includeBaseCheckBox;
    QLabel *macroNameLabel;
    QLabel *baseIncludeLabel;
    QLineEdit *macroNameLineEdit;
    QLineEdit *baseIncludeLineEdit;
};
