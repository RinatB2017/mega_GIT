//--------------------------------------------------------------------------------
#include <QtGui>
#include <QDebug>
//--------------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0) : QMainWindow(parent)
    {
        m_log = new QTextBrowser;

        m_lineEdit = new QLineEdit;
        connect(m_lineEdit, SIGNAL(returnPressed()), this, SLOT(send()));

        QPushButton* sendButton = new QPushButton(tr("Send"));
        connect(sendButton, SIGNAL(clicked()), this, SLOT(send()));

        QPushButton* receiveButton = new QPushButton(tr("Read"));
        connect(receiveButton, SIGNAL(clicked()), this, SLOT(readDataOnButtonPress()));

        QHBoxLayout* blo = new QHBoxLayout;
        blo->addWidget(m_lineEdit);
        blo->addWidget(sendButton);
        blo->addWidget(receiveButton);

        QWidget* cw = new QWidget;
        QVBoxLayout* hlo = new QVBoxLayout;
        cw->setLayout(hlo);
        hlo->addWidget(m_log);
        hlo->addLayout(blo);

        m_engine = new QProcess(this);
        m_engine->setProcessChannelMode(QProcess::MergedChannels);
        m_engine->setReadChannel(QProcess::StandardOutput);
        connect(m_engine, SIGNAL(readyRead()), this, SLOT(readData()));
        m_engine->start("/usr/games/bin/gnuchess", QStringList() << "-x");

        setCentralWidget(cw);
    }

    ~MainWindow()
    {
        m_engine->terminate();
        m_engine->kill();
    }

private:
    QTextBrowser* m_log;
    QProcess* m_engine;
    QLineEdit* m_lineEdit;

private slots:
    void readData()
    {
        QByteArray ba;

        ba = m_engine->readAllStandardOutput().trimmed();
        if (!ba.trimmed().isEmpty())
        {
            m_log->append(ba);
        }

        ba = m_engine->readAllStandardError().trimmed();
        if (!ba.trimmed().isEmpty())
        {
            m_log->append(ba);
        }
    }

    void readDataOnButtonPress()
    {
        QString info;

        info.append("----------------\n");
        info.append(QString("bytes available: %1\n").arg(m_engine->bytesAvailable()));
        info.append("----------------\n");

        m_log->append(info);

        readData();
    }

    void send()
    {
        QString text = m_lineEdit->text().trimmed();
        m_lineEdit->clear();

        if (text.isEmpty())
            return;

        m_engine->write(text.toAscii());
        m_log->append(text);
    }
};
//--------------------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MainWindow mw;
    mw.setWindowTitle(QObject::tr("QProcess test"));
    mw.show();
    return app.exec();
}
//--------------------------------------------------------------------------------
#include "main.moc"

