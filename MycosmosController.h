#ifndef MYCOSMOSCONTROLLER_H
#define MYCOSMOSCONTROLLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>


class MycosmosController : public QObject
{
    Q_OBJECT
public:
    MycosmosController(QObject *parent = 0);
    void downloadPage();
    void getPage(const QString &pageUrl,const QString &refererUrl);
    void postPage(const QString &pageUrl,const QString &refererUrl,const QString &params);
    QString getViewState();
    QString getViewState(QString &htmlResponse);
    void addRequestHeaders(QNetworkRequest &requesti,const QString &referer);
    void setCookieData(QVariant &cookies);
    QVariant getCookieData();
    QString getHtmlResponse();
signals:
    void successfulResponse();
    void failedResponse(QNetworkReply::NetworkError errorCode);
public slots:
    void downloadFinished(QNetworkReply *reply);
private:
    QUrl url;
    QVariant cookieData;
    QString viewStateString;
    QString htmlReplyText;
    QNetworkReply *nReply;
    QNetworkAccessManager *nManager;
};

#endif // MYCOSMOSCONTROLLER_H
