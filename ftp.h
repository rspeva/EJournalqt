#ifndef FTP_H
#define FTP_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QUrl>
#include <QDebug>

class Ftp : public QObject
{
    Q_OBJECT
public:
    explicit Ftp(QObject *parent = 0);

signals:
    void onReadyDownload();
    void onReadyUpload();
    void onErrorDownload();

public slots:
    void downloadDB();
    void uploadDB();
    void onResultDownload(QNetworkReply *reply);
    void onResultUpload (QNetworkReply *reply);

private:
    QNetworkAccessManager *managerDownload;
    QNetworkAccessManager *managerUpload;
    QFile *file;
};

#endif // FTP_H
