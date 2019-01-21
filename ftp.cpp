 #include "ftp.h"



Ftp::Ftp(QObject *parent) : QObject(parent)
{
    managerDownload = new QNetworkAccessManager();
    connect(managerDownload, &QNetworkAccessManager::finished, this, &Ftp::onResultDownload);

    managerUpload = new QNetworkAccessManager();
    connect(managerUpload, &QNetworkAccessManager::finished, this, &Ftp::onResultUpload);
}

void Ftp::downloadDB()
{
    QUrl url("ftp://dnz143zp.zzz.com.ua/dnz143zp.zzz.com.ua/ejournal/database.db3");
    url.setUserName("spevakov");
    url.setPassword("roma1997");
    url.setPort(21);
    managerDownload->get(QNetworkRequest(url));
}

void Ftp::onResultDownload(QNetworkReply *reply)
{
    if(!reply->error()){
        QFile *file = new QFile("D:\\database.db3");
        if(file->open(QFile::WriteOnly)){
                   file->write(reply->readAll());
                   file->close();
        }
        emit onReadyDownload();
    } emit onErrorDownload();
}

void Ftp::uploadDB()
{
    file = new QFile("D:\\database.db3");
    QUrl url("ftp://dnz143zp.zzz.com.ua/dnz143zp.zzz.com.ua/ejournal/database.db3");
    url.setUserName("spevakov");
    url.setPassword("roma1997");
    url.setPort(21);
    if (file->open(QIODevice::ReadOnly))
    managerUpload->put(QNetworkRequest(url), file);
}

void Ftp::onResultUpload(QNetworkReply *reply)
{
    if (!reply->error())
       {
           file->close();
           file->deleteLater();
           reply->deleteLater();
       }
        emit onReadyUpload();
}

