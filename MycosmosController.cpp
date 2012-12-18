#include "MycosmosController.h"
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QDebug>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QList>
#include <stdio.h>


MycosmosController::MycosmosController(QObject *parent) :
    QObject(parent)
{
    nManager = new QNetworkAccessManager(this);

    connect(nManager,SIGNAL(finished(QNetworkReply*)),SLOT(downloadFinished(QNetworkReply*)));
}

void MycosmosController::downloadPage(){
    //    QString urlString = "http://www.google.gr";
    QString urlString = "http://mail.mycosmos.gr/mycosmos/login.aspx";
    QUrl urliki(urlString);

    QNetworkRequest nRequest(urlString);

    nManager->get(nRequest);

}




void MycosmosController::downloadFinished(QNetworkReply *reply){
    QUrl url = reply->url();

    if(reply->error() == QNetworkReply::NoError) {
        // Get the http status code
        int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();


        if (v >= 200 && v < 300) // Success
        {
            // Here we got the final reply
            QString replyText = QString::fromUtf8(reply->readAll());

            //printf("Reply data: %s\n",qPrintable(replyText));

            QList<QNetworkCookie>  cookies = nManager->cookieJar()->cookiesForUrl(url);
            for(int i=0; i<cookies.count(); i++){
                printf("COOKIE %d Name:%s Value:%s\n" ,i,qPrintable(QString::fromUtf8(cookies.at(i).name())),qPrintable(QString::fromUtf8(cookies.at(i).value())) );
            }
            printf("COOKIES for %s\n%s\n" ,qPrintable(url.host()) ,qPrintable(QString::number(cookies.count())));

        }
        else if (v >= 300 && v < 400) // Redirection
        {
            // Get the redirection url

            QUrl newUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            // Because the redirection url can be relative,
            // we have to use the previous one to resolve it
            newUrl = reply->url().resolved(newUrl);

            //            QNetworkAccessManager *manager = nReply->manager();
            //            QNetworkRequest redirection(newUrl);
            //            QNetworkReply *newReply = manager->get(redirection);

            return; // to keep the manager for the next request
        }
    }
    else
    {
        // Error
        printf("Download of %s failed: %s\n",url.toEncoded().constData(),qPrintable(reply->errorString()));
    }

    printf("Url encoded Νένα 1:%s\n" ,qPrintable(QUrl::toPercentEncoding("Νένα 1",NULL,NULL)));
}

void MycosmosController::setRequestHeaders(QNetworkRequest &requesti,QString &referer){
    requesti.setRawHeader("User-Agent","Mozilla/5.0 (X11; Linux i686; rv:2.0b12) Gecko/20110222 Firefox/4.0b12");
    requesti.setRawHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    requesti.setRawHeader("Accept-Language","el-gr,el;q=0.8,en-us;q=0.5,en;q=0.3");
    requesti.setRawHeader("Accept-Encoding", "gzip, deflate");
    requesti.setRawHeader("Accept-Charset","ISO-8859-7,utf-8;q=0.7,*;q=0.7");

    requesti.setRawHeader("Content-Type","application/x-www-form-urlencoded");

    if (!referer.isNull() && !referer.isEmpty()) {
        requesti.setRawHeader("Referer", referer.toUtf8());
    }
}
