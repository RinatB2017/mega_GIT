#include <QCoreApplication>
#include <Qca-qt5/QtCrypto/QtCrypto>
#include <QFile>
#include <QDir>
#include <iostream>

int main(int argc, char *argv[])
{
    QCA::Initializer init;
    
    // Get file path and file
    QDir path(QDir::currentPath());
    path.cd(QDir::homePath() + "/QtProjects/ConsoleEncryption");
    QString filepath = path.absolutePath() + "/" + "data.json";
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly)){}
    if(!file.isReadable()) { std::cout << "Not Readable" << std::endl; }
    //    QJsonParseError error;
    //    QJsonDocument jsonFile = QJsonDocument::fromJson(file.readAll(), &error);
    //    std::cout << jsonFile.toJson().data() << std::endl;
    QString serial = file.readAll();
    
    
    QCA::SecureArray arg = serial.toUtf8().data();
    //    QCA::SecureArray arg = jsonFile.toJson().data();
    if (!QCA::isSupported("pkey") || !QCA::PKey::supportedIOTypes().contains(QCA::PKey::RSA)) {
        std::cout << "RSA not supported!\n";
    } else {
        /*** CREATE PASSPHRASE ************************************************/
        QCA::SecureArray passPhrase = "ManterWeigher";

        /*** READ PRIVATE KEY FROM PEM FILE ***********************************/
        QCA::ConvertResult conversionResultPrivate;
        QCA::PrivateKey privateKey = QCA::PrivateKey::fromPEMFile( "privateKey.pem",
                                                                   passPhrase,
                                                                   &conversionResultPrivate);
        
        if (! (QCA::ConvertGood == conversionResultPrivate) ) {
            std::cout << "Private key read failed" << std::endl;
            return 1;
        }
        
        /*** READ PUBLIC KEY FROM PEM FILE ************************************/
        QCA::ConvertResult conversionResultPublic;
        QCA::PublicKey publicKey = QCA::PublicKey::fromPEMFile("publicKey.pem", &conversionResultPublic);
        if (! (QCA::ConvertGood == conversionResultPublic) ) {
            std::cout << "Public key read failed" << std::endl;
            return 1;
        }

        /*** ENCRYPT DATA WITH PUBLIC KEY *************************************/
        QCA::SecureArray result = publicKey.encrypt(arg, QCA::EME_PKCS1_OAEP);
        if(result.isEmpty()) {
            std::cout << "Error encrypting" << std::endl;
            return 1;
        }
        
        QString rstr = QCA::arrayToHex(result.toByteArray());
        std::cout << "\"" << arg.data() << "\" \n encrypted with RSA is :\"";
        std::cout << qPrintable(rstr) << "\"" << std::endl;
        
        /*** DECRYPT DATA WITH PRIVATE KEY ************************************/
        QCA::SecureArray decrypt;
        if(0 == privateKey.decrypt(result, &decrypt, QCA::EME_PKCS1_OAEP)) {
            std::cout << "Error decrypting.\n";
        }
        
        // output the resulting decrypted string
        std::cout << "\"" << qPrintable(rstr) << "\" \n decrypted with RSA is :\"";
        std::cout << decrypt.data() << "\"" << std::endl;
    }
    
    QCoreApplication a(argc, argv);
    
    return a.exec();
}
