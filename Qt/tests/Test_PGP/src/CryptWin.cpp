
#include <QtGui>
#include <QtCrypto>

#include "CryptWin.h"
#include "ui_CryptWin.h"

Q_DECLARE_METATYPE(QCA::KeyStoreEntry)

CryptWindow::CryptWindow(QWidget * p) :
	QMainWindow(p),
	ui(new Ui::CryptWindow)
{
	ui->setupUi(this);
	QFont mono; mono.setFamily("Bitstream Vera Sans Mono");
	QTextCharFormat fmt = ui->pte_src->currentCharFormat();
	fmt.setFont(mono);
	ui->pte_dst->setCurrentCharFormat(fmt);
	ui->pte_src->setCurrentCharFormat(fmt);
	ui->pte_src->setPlainText("Hello World!");

	QCA::KeyStoreManager::start();
	QCA::KeyStoreManager ksm(this);
	ksm.waitForBusyFinished();

	QCA::KeyStore pgpks( QString("qca-gnupg"), &ksm );

	foreach(const QCA::KeyStoreEntry kse, pgpks.entryList()) {
		QString text = kse.name()+" "+kse.id();
		QVariant v; v.setValue(kse);
		ui->cb_to->addItem(text, v);
		if (!kse.pgpSecretKey().isNull())
			ui->cb_my->addItem(text, v);
	}
}

CryptWindow::~CryptWindow() { delete ui; }

void CryptWindow::encrypt() {
	QVariant v_my = ui->cb_my->itemData(ui->cb_my->currentIndex());
	QVariant v_to = ui->cb_to->itemData(ui->cb_to->currentIndex());
	if (!v_my.isValid()) { ui->pte_dst->setPlainText("Invalid src key"); return; }
	if (!v_to.isValid()) { ui->pte_dst->setPlainText("Invalid dst key"); return; }
	QCA::KeyStoreEntry kse_my = v_my.value<QCA::KeyStoreEntry>();
	QCA::KeyStoreEntry kse_to = v_to.value<QCA::KeyStoreEntry>();

	QCA::SecureMessageKey to;
	to.setPGPSecretKey( kse_my.pgpSecretKey() );
	to.setPGPPublicKey( kse_to.pgpPublicKey() );

	QCA::OpenPGP pgp;
	QCA::SecureMessage msg(&pgp);

	msg.setRecipient(to);
	msg.setFormat(QCA::SecureMessage::Ascii);
	msg.startEncrypt();
	msg.update(ui->pte_src->toPlainText().toUtf8());
	msg.end();
	msg.waitForFinished(2000);

	QByteArray crpt = msg.read();
	ui->pte_dst->setPlainText(QString::fromUtf8(crpt));
}
