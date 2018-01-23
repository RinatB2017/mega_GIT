#include "createdialog.h"
#include "ui_createdialog.h"

CreateDialog::CreateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDialog)
{
    ui->setupUi(this);
    ui->formLayout->setContentsMargins(0, 0, 0, 0);
    connect(&nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(loadVersionList(QNetworkReply*)));

    qDebug() << "Url: " << MINETEST_URL.toString();
    QNetworkRequest request(MINETEST_URL);
    nam.get(request);

}

CreateDialog::~CreateDialog()
{
    delete ui;
}

void CreateDialog::loadVersionList(QNetworkReply *reply)
{
    qDebug() << "Data size: " << reply->size();
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonArray array = document.array();

    qDebug() << "Versions: " << array.size();
    for (int i = 0; i < array.size(); i++)
        ui->versionList->addItem(array.at(i).toObject().value("tag_name").toString());
    reply->deleteLater();
    ui->buttonBox->setEnabled(true);
}

void CreateDialog::newProfile(ProfileManager *manager)
{
    CreateDialog dialog;
    if (dialog.exec() != QDialog::Accepted)
        return;

    if (!manager->addProfile(Profile(dialog.ui->nameBox->text(), dialog.ui->versionList->currentText())))
        QMessageBox::critical(0, "Error", "A profile with the same name already exists.");
}
