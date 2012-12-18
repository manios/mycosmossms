#ifndef MYCOSMOSCONTROLLER_H
#define MYCOSMOSCONTROLLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>


class MycosmosController : public QObject
{
    Q_OBJECT
public:
    MycosmosController(QObject *parent = 0);
    void downloadPage();
    QString getPage(QString &pageUrl,QString &refererUrl);
    QString postPage(QString &pageUrl,QString &refererUrl,QString &params);
    QString getViewState(QString &htmlResponse);
    void setRequestHeaders(QNetworkRequest &requesti,QString &referer);
    int connectToHost(QString &username,QString &password);
    int sendMessage(QString &telephoneNumber,QString &messageText);
    int disconnectFromHost(QString &username,QString &password);
    bool isConnected();

signals:
    
public slots:
    void downloadFinished(QNetworkReply *reply);
private:
    QUrl url;
    QNetworkAccessManager *nManager;
};

#endif // MYCOSMOSCONTROLLER_H
