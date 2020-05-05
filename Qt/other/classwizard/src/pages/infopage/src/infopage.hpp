
#include <QWizardPage>
#include <QVBoxLayout>
#include <QLabel>

class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    IntroPage(QWidget *parent = nullptr);

private:
    QLabel *label;
};
