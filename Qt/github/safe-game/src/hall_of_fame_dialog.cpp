#include "hall_of_fame_dialog.h"

#include <QDir>
#include <QInputDialog>
#include <QFile>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QTableWidget>
#include <QTextStream>
#include <QVBoxLayout>

#include "utils.h"

namespace
{
    const char* FileName = "hall_of_fame.dat";
}

void safe::HallOfFameDialog::showHall()
{
    HallOfFameDialog dialog;
    dialog.loadData();
    dialog.fillTable();
    dialog.exec();
}

void safe::HallOfFameDialog::showHall(int newTime)
{
    HallOfFameDialog dialog;
    dialog.loadData();
    dialog.checkNewRecord(newTime);
    dialog.fillTable();
    dialog.exec();
}

safe::HallOfFameDialog::HallOfFameDialog(QWidget* parent)
    : QDialog(parent)
{
    table_ = new QTableWidget();
    table_->setColumnCount(2);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->setSelectionMode(QAbstractItemView::NoSelection);
    table_->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Name")));
    table_->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Time")));
    table_->horizontalHeader()->setStretchLastSection(true);

    auto ok = new QPushButton(tr("Ok"));
    connect(ok, &QPushButton::clicked, this, &QDialog::accept);

    auto controls = new QHBoxLayout();
    controls->addStretch();
    controls->addWidget(ok);
    controls->addStretch();

    auto layout = new QVBoxLayout();
    layout->addWidget(table_);
    layout->addLayout(controls);

    setLayout(layout);
}

safe::HallOfFameDialog::~HallOfFameDialog()
{
    saveData();
}

void safe::HallOfFameDialog::loadData()
{
    QFile file(FileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        setDefaultData();
        return;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");

    int n = 0;
    while(!in.atEnd())
    {
        const QString name = in.readLine();
        if (in.atEnd())
        {
            setDefaultData();
            return;
        }

        const QString timeAsText = in.readLine();

        bool ok = false;
        const int time = timeAsText.toInt(&ok);
        if (!ok || time <= 0)
        {
            setDefaultData();
            return;
        }

        nameByTime_.insert(time, name);

        ++n;
        if (n >= MaxNames)
        {
            return;
        }
    }

    if (nameByTime_.size() < MaxNames)
    {
        setDefaultData();
    }
}

void safe::HallOfFameDialog::saveData()
{
    QFile file(FileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");

    for (auto it = nameByTime_.constBegin(), end = nameByTime_.constEnd(); it != end; ++it)
    {
        out << it.value() << endl;
        out << it.key() << endl;
    }
}

void safe::HallOfFameDialog::fillTable()
{
    table_->setRowCount(nameByTime_.size());

    int row = 0;
    for (auto it = nameByTime_.constBegin(), end = nameByTime_.constEnd(); it != end; ++it)
    {
        table_->setItem(row, 0, new QTableWidgetItem(it.value()));
        table_->setItem(row, 1, new QTableWidgetItem(secondsToString(it.key())));
        ++row;
    }

    table_->resizeRowsToContents();
}

void safe::HallOfFameDialog::setDefaultData()
{
    nameByTime_.clear();
    nameByTime_.insert(100, "Иван");
    nameByTime_.insert(115, "Анна");
    nameByTime_.insert(150, "Татьяна");
    nameByTime_.insert(150, "Ольга");
    nameByTime_.insert(170, "Антон");
    nameByTime_.insert(180, "Ирина");
    nameByTime_.insert(195, "Владимир");
    nameByTime_.insert(250, "Олег");
    nameByTime_.insert(300, "Наталья");
    nameByTime_.insert(500, "Ксения");
}

void safe::HallOfFameDialog::checkNewRecord(int time)
{
    if (nameByTime_.upperBound(time) != nameByTime_.constEnd())
    {
        bool ok;
        const QString name = QInputDialog::getText(nullptr,
            tr("Wow!"), tr("This is a new record! Your name:"), QLineEdit::Normal, "", &ok);
        if (ok && !name.isEmpty())
        {
            nameByTime_.insert(time, name);
            nameByTime_.remove(nameByTime_.lastKey());
        }
    }
}
