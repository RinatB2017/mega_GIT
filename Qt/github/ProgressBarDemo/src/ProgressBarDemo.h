#ifndef PROGRESSBARDEMO
#define PROGRESSBARDEMO

#include <QWidget>
#include <QTimer>

static const QStringList TABLE_COLUMN_LABELS = QStringList() << "Progress";
static const int PROGRESS_STEP = 5;
static const int PROGRESS_DELAY_MSEC = 100;
static const int MAX_PROGRESS_VALUE = 100;

class QTableWidget;
class QPushButton;

class ProgressBarDemo : public QWidget
{
    Q_OBJECT

public:
    ProgressBarDemo( QWidget* parent = 0 );
    ~ProgressBarDemo();

private slots:
    void addRow();
    void onProgress();

private:
    QTableWidget* m_table;
    QPushButton* m_btnAdd;

    QTimer m_timer;

    int m_currentRow;
    int m_currentProgress;

};

#endif
