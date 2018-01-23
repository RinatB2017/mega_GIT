#ifndef CREATEDIALOG_H
#define CREATEDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QEventLoop>
#include <QMessageBox>
#include "profilemanager.h"

namespace Ui {
    class CreateDialog;
}

class CreateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDialog(QWidget *parent = 0);
    static void newProfile(ProfileManager*);

    ~CreateDialog();

private:
    Ui::CreateDialog *ui;
    QNetworkAccessManager nam;

    QUrl MINETEST_URL = QUrl("https://api.github.com/repos/minetest/minetest/releases");

private slots:
    void loadVersionList(QNetworkReply*);
};

#endif // CREATEDIALOG_H
