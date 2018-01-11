#include "bookmarksmanager.h"

BookmarksManager::BookmarksManager(QWidget *parent) :
    QFrame(parent)
{
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    this->setStyleSheet("*{background:rgba(235,235,235,1); color: black;}");

    tableView = new QTableView;
    tableView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setAlternatingRowColors(true);
    tableView->setVerticalScrollMode(QTableView::ScrollPerPixel);
    tableView->setHorizontalScrollMode(QTableView::ScrollPerPixel);
    tableView->setStyleSheet("*{background:rgba(235,235,235,1); color: black; font-size: 14px; min-width: 50px;} QLineEdit {background: white; border-radius: 0px; border: none;}");
    QHeaderView *header = tableView->horizontalHeader();
    // header->setStretchLastSection(true);
    header->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(tableView);

    textView = new QTextEdit;
    textView->setReadOnly(true);
    textView->setText(tr("Sorry ! You have no bookmarks ;("));
    textView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    mainLayout->addWidget(textView);

    model = new QStandardItemModel;
    model->setHeaderData(0,Qt::Horizontal,tr("ID"));
    model->setHeaderData(1,Qt::Horizontal,tr("Name"));
    model->setHeaderData(2,Qt::Horizontal,tr("Url"));
    tableView->setModel(model);

    btnDelete = new QPushButton(QIcon(":/img/close.png"),"",tableView);
    //btnDelete->setStyleSheet("QPushButton {border: 1px solid rgba(0,0,0,.5); background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 rgb(200,0,0), stop: 1.0 rgb(150,0,0)); border-radius: 25px; min-width: 0px;} QPushButton:hover {background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 rgb(255,100,100),stop: 0.2 red, stop: 1.0 rgb(200,0,0));} QPushButton:pressed {background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 rgb(200,0,0), stop: 1.0 red);}");
    btnDelete->setStyleSheet("QPushButton {border: 1px solid rgba(0,0,0,.5); background: rgb(200,0,0); border-radius: 25px; min-width: 0px;} QPushButton:hover {background: rgb(255,100,100);} QPushButton:pressed {background: red;}");
    btnDelete->setFixedSize(50,50);
    btnDelete->move(tableView->geometry().width()-(btnDelete->geometry().width()+15),tableView->geometry().height()-(btnDelete->geometry().height()+15));
    connect(btnDelete,SIGNAL(clicked()),this,SLOT(deleteItems()));
    btnDelete->hide();

    QColor color = QColor::fromRgb(0,0,0);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(10);
    effect->setColor(color);
    effect->setOffset(0,0);
    //btnDelete->setGraphicsEffect(effect);

    updateItems();
}

void BookmarksManager::updateItems()
{
    model->clear();

    QSettings *settings = new QSettings;
    settings->beginGroup("Bookmarks");

    if(settings->childGroups().count()==0) // Si aucun favoris
    {
        tableView->hide();
        textView->show();
    }
    else
    {
        tableView->show();
        textView->hide();

        for(int i(1);i<=settings->childGroups().count();i++)
        {
            int item = i+1000;

            QStandardItem *id = new QStandardItem(QString::number(item));
            id->setIcon(settings->value(QString::number(item)+"/Icon").value<QIcon>());
            model->setItem(i-1, 0, id);
            model->setItem(i-1, 1, new QStandardItem(settings->value(QString::number(item)+"/Name").toString()));
            model->setItem(i-1, 2, new QStandardItem(settings->value(QString::number(item)+"/Url").toString()));
        }

        model->setHeaderData(0,Qt::Horizontal,tr("ID"));
        model->setHeaderData(1,Qt::Horizontal,tr("Name"));
        model->setHeaderData(2,Qt::Horizontal,tr("Url"));
    }

    settings->endGroup();
}

void BookmarksManager::saveBookmarks()
{
    /*QSettings *settings = new QSettings;
    settings->beginGroup("Bookmarks");

    QStandardItemModel *tableModel = qobject_cast<QStandardItemModel *>(tableView->model());

   // tableView->model()->rowCount(); // Nbre de ligne

    for(int i(1);i<=settings->childGroups().count();i++)
    {
        //int item = i+1000;
        QString id = tableModel->item(i-1,0)->text();
        QString title = tableModel->item(i-1,1)->text();
        QString url = tableModel->item(i-1,2)->text();

        settings->setValue(id+"/Name",title);
        settings->setValue(id+"/Url",QUrl(url));
    }

    settings->endGroup();*/

    QSettings *settings = new QSettings;
    settings->beginGroup("Bookmarks");

    QStandardItemModel *tableModel = qobject_cast<QStandardItemModel *>(tableView->model());

    for(int i(1);i<=tableView->model()->rowCount();i++)
    {
        //int item = i+1000;
        QString id = tableModel->item(i-1,0)->text();

        /*
        QMessageBox::information(this,"Info","Item : "+QString::number(item)+" - ID : " + id);

        if(item==id.toInt()-1)
        {
            // Si la ligne i correspond a un item-1
            settings->remove(QString::number(item));
            item==item+1;
        }

        QMessageBox::information(this,"Info","Item : "+QString::number(item)+" - ID : " + id);
        */

        QString title = tableModel->item(i-1,1)->text();
        QString url = tableModel->item(i-1,2)->text();

        settings->setValue(id+"/Name",title);
        settings->setValue(id+"/Url",QUrl(url));
    }

    settings->endGroup();
}

void BookmarksManager::resizeEvent(QResizeEvent *event)
{
    btnDelete->move(tableView->geometry().width()-(btnDelete->geometry().width()+15),tableView->geometry().height()-(btnDelete->geometry().height()+15));
    event->accept();
}

void BookmarksManager::deleteItems()
{
    QItemSelectionModel *selection = tableView->selectionModel();
    QModelIndexList listeSelections = selection->selectedRows();
    QMessageBox::information(this,"listSelect",QString::number(listeSelections.size()));

    for (int i = 0 ; i < listeSelections.size() ; i++)
    {
        model->removeRow(listeSelections.at(i).row());
        tableView->update();
    }

    // Supprimer les lignes vides
}
