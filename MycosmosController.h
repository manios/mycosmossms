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
    void getPage(QString &pageUrl,QString &refererUrl);
    void postPage(QString &pageUrl,QString &refererUrl,QString &params);
    QString getViewState(QString &htmlResponse);
    void addRequestHeaders(QNetworkRequest &requesti,QString &referer);
    int connectToHost(QString &username,QString &password);
    int sendMessage(QString &telephoneNumber,QString &messageText);
    int disconnectFromHost(QString &username,QString &password);
    bool isConnected();

signals:
    void successfulResponse();
public slots:
    void downloadFinished(QNetworkReply *reply);
private:
    QUrl url;
    QVariant cookieData;
    QNetworkReply *nReply;
    QNetworkAccessManager *nManager;
};

#endif // MYCOSMOSCONTROLLER_H
