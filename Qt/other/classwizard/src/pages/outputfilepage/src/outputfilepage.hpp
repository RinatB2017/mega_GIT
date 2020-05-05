
#include <QWizardPage>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDir>

class OutputFilesPage : public QWizardPage
{
    Q_OBJECT

public:
    OutputFilesPage(QWidget *parent = nullptr);

protected:
    void initializePage() override;

private:
    QLabel *outputDirLabel;
    QLabel *headerLabel;
    QLabel *implementationLabel;
    QLineEdit *outputDirLineEdit;
    QLineEdit *headerLineEdit;
    QLineEdit *implementationLineEdit;
};
