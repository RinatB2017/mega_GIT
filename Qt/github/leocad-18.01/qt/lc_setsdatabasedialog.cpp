#include "lc_global.h"
#include "lc_setsdatabasedialog.h"
#include "ui_lc_setsdatabasedialog.h"
#include <QNetworkRequest>
#include <QNetworkReply>

#ifdef Q_OS_WIN

lcHttpReply::lcHttpReply(QObject* Parent, const QString& URL)
	: QThread(Parent)
{
	mError = true;
	mAbort = false;
	mURL = URL;
}

void lcHttpReply::run()
{
	HINTERNET Session = nullptr;
	HINTERNET Request = nullptr;

	if (sizeof(wchar_t) != sizeof(QChar))
		return;

	Session = InternetOpen(L"LeoCAD", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (!Session)
		return;

	Request = InternetOpenUrl(Session, (WCHAR*)mURL.data(), NULL, 0, 0, 0);
	if (!Request)
	{
		InternetCloseHandle(Session);
		return;
	}

	for (;;)
	{
		char Buffer[1024];
		DWORD BytesRead;

		if (mAbort)
			break;

		if (!InternetReadFile(Request, Buffer, sizeof(Buffer), &BytesRead))
			break;

		if (BytesRead)
			mBuffer.append(Buffer, BytesRead);
		else
		{
			mError = false;
			break;
		}
	}

	InternetCloseHandle(Request);
	InternetCloseHandle(Session);
}

#endif

lcSetsDatabaseDialog::lcSetsDatabaseDialog(QWidget* Parent)
	: QDialog(Parent),
    ui(new Ui::lcSetsDatabaseDialog)
{
	ui->setupUi(this);
	ui->SearchEdit->installEventFilter(this);

	connect(ui->SetsTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(accept()));
	connect(this, SIGNAL(finished(int)), this, SLOT(Finished(int)));
#ifndef Q_OS_WIN
	connect(&mNetworkManager, SIGNAL(finished(QNetworkReply*)), SLOT(DownloadFinished(lcHttpReply*)));
#endif

	mKeyListReply = RequestURL("http://www.leocad.org/rebrickable.json");
}

lcSetsDatabaseDialog::~lcSetsDatabaseDialog()
{
	delete ui;
}

lcHttpReply* lcSetsDatabaseDialog::RequestURL(const QString& URL)
{
#ifdef Q_OS_WIN
	lcHttpReply* Reply = new lcHttpReply(this, URL);
	connect(Reply, &QThread::finished, [this, Reply] { DownloadFinished(Reply); });
	Reply->start();
	return Reply;
#else
	return mNetworkManager.get(QNetworkRequest(QUrl(URL)));
#endif
}

QString lcSetsDatabaseDialog::GetSetName() const
{
	QTreeWidgetItem* Current = ui->SetsTree->currentItem();
	return Current ? Current->text(0) : QString();
}

QString lcSetsDatabaseDialog::GetSetDescription() const
{
	QTreeWidgetItem* Current = ui->SetsTree->currentItem();
	return Current ? Current->text(1) : QString();
}

bool lcSetsDatabaseDialog::eventFilter(QObject* Object, QEvent* Event)
{
	if (Event->type() == QEvent::KeyPress)
	{
		QKeyEvent* KeyEvent = static_cast<QKeyEvent*>(Event);

		int Key = KeyEvent->key();
		if (Key == Qt::Key_Return || Key == Qt::Key_Enter)
		{
			KeyEvent->accept();
			on_SearchButton_clicked();
			return true;
		}
	}

	return QDialog::eventFilter(Object, Event);
}

void lcSetsDatabaseDialog::accept()
{
	QTreeWidgetItem* Current = ui->SetsTree->currentItem();
	if (!Current)
	{
		QMessageBox::information(this, "LeoCAD", tr("Please select a set from the list."));
		return;
	}

	QString SetNum = Current->text(0);

	QProgressDialog ProgressDialog(this);
	ProgressDialog.setWindowTitle(tr("Downloading"));
	ProgressDialog.setLabelText(tr("Downloading set inventory"));
	ProgressDialog.setMaximum(0);
	ProgressDialog.setMinimum(0);
	ProgressDialog.setValue(0);
	ProgressDialog.show();

	int KeyIndex = QTime::currentTime().msec() % mKeys.size();
	QString DownloadUrl = QString("https://rebrickable.com/api/v3/lego/sets/%1/parts/?key=%2").arg(SetNum, mKeys[KeyIndex]);

	mInventoryReply = RequestURL(DownloadUrl);

	while (mInventoryReply)
	{
		QApplication::processEvents();

		if (ProgressDialog.wasCanceled())
		{
			mInventoryReply->abort();
			mInventoryReply->deleteLater();
			mInventoryReply = nullptr;
			return;
		}
	}

	QDialog::accept();
}

void lcSetsDatabaseDialog::Finished(int Result)
{
	Q_UNUSED(Result);

	if (mKeyListReply)
	{
		mKeyListReply->abort();
		mKeyListReply->deleteLater();
	}
}

void lcSetsDatabaseDialog::on_SearchButton_clicked()
{
	QString Keyword = ui->SearchEdit->text();

	if (Keyword.isEmpty())
	{
		QMessageBox::information(this, "LeoCAD", tr("Keyword cannot be empty."));
		return;
	}

	QProgressDialog ProgressDialog(this);
	ProgressDialog.setWindowTitle(tr("Searching"));
	ProgressDialog.setLabelText(tr("Connecting to server"));
	ProgressDialog.setMaximum(0);
	ProgressDialog.setMinimum(0);
	ProgressDialog.setValue(0);
	ProgressDialog.show();

	while (mKeyListReply)
	{
		QApplication::processEvents();

		if (ProgressDialog.wasCanceled())
			return;
	}

	if (mKeys.isEmpty())
		return;

	int KeyIndex = QTime::currentTime().msec() % mKeys.size();
	QString SearchUrl = QString("https://rebrickable.com/api/v3/lego/sets/?search=%1&key=%2").arg(Keyword, mKeys[KeyIndex]);

	mSearchReply = RequestURL(SearchUrl);

	while (mSearchReply)
	{
		QApplication::processEvents();

		if (ProgressDialog.wasCanceled())
		{
			mSearchReply->abort();
			mSearchReply->deleteLater();
			mSearchReply = nullptr;
			break;
		}
	}
}

void lcSetsDatabaseDialog::DownloadFinished(lcHttpReply* Reply)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
	if (Reply == mKeyListReply)
	{
		if (!Reply->error())
		{
			QJsonDocument Document = QJsonDocument::fromJson(Reply->readAll());
			QJsonObject Root = Document.object();

			int Version = Root["Version"].toInt();

			if (Version == 1)
			{
				QJsonArray Keys = Root["Keys"].toArray();

				for (const QJsonValue& Key : Keys)
					mKeys.append(Key.toString());
			}
		}

		if (mKeys.isEmpty())
		{
			QMessageBox::information(this, "LeoCAD", tr("Error connecting to server."));
			close();
		}

		mKeyListReply = nullptr;
	}
	else if (Reply == mSearchReply)
	{
		QTreeWidget* SetsTree = ui->SetsTree;
		SetsTree->clear();

		if (!Reply->error())
		{
			QJsonDocument Document = QJsonDocument::fromJson(Reply->readAll());
			QJsonObject Root = Document.object();

			QJsonArray Sets = Root["results"].toArray();
			for (const QJsonValue& Set : Sets)
			{
				QJsonObject SetObject = Set.toObject();
				QStringList SetInfo;

				SetInfo << SetObject["set_num"].toString();
				SetInfo << SetObject["name"].toString();
				SetInfo << QString::number(SetObject["year"].toInt());
				SetInfo << QString::number(SetObject["num_parts"].toInt());

				new QTreeWidgetItem(ui->SetsTree, SetInfo);
			}

			if (!Sets.isEmpty())
			{
				SetsTree->resizeColumnToContents(0);
				SetsTree->resizeColumnToContents(1);
				SetsTree->resizeColumnToContents(2);
				SetsTree->resizeColumnToContents(3);
				SetsTree->setCurrentItem(SetsTree->topLevelItem(0));
			}
		}

		mSearchReply = nullptr;
	}
	else if (Reply == mInventoryReply)
	{
		if (!Reply->error())
			mInventory = Reply->readAll();
		else
			QMessageBox::information(this, "LeoCAD", tr("Error downloading set inventory."));

		mInventoryReply = nullptr;
	}
#endif

	Reply->deleteLater();
}
