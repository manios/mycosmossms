#include "MycosmosController.h"
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <stdio.h>

MycosmosController::MycosmosController(QObject *parent) :
    QObject(parent)
{
    connect(&nManager,SIGNAL(finished(QNetworkReply*)),SLOT(downloadFinished(QNetworkReply*)));
}

void MycosmosController::downloadPage(){
    QString urlString = "http://www.google.gr";
    QUrl urliki(urlString);

    QNetworkRequest nRequest(urlString);
    nManager.get(nRequest);



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

            printf("Reply data: %s\n",qPrintable(replyText));
            //              qDebug()<<replyText;
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

}
