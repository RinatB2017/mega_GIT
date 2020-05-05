
#include <QWizardPage>
#include <QVBoxLayout>
#include <QLabel>

class ConclusionPage : public QWizardPage
{
    Q_OBJECT

public:
    ConclusionPage(QWidget *parent = nullptr);

protected:
    void initializePage() override;

private:
    QLabel *label;
};
