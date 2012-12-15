#include "mycosmoscontroller.h"
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <stdio.h>

mycosmoscontroller::mycosmoscontroller(QObject *parent) :
    QObject(parent)
{
    connect(&nManager,SIGNAL(finished(QNetworkReply*)),SLOT(downloadFinished(QNetworkReply*)));
}

void mycosmoscontroller::downloadPage(){
    QString urlString = "www.google.gr";
    QUrl urliki(urlString);

    QNetworkRequest nRequest(urlString);
    nReply = nManager.get(nRequest);

}

void mycosmoscontroller::downloadFinished(QNetworkReply *reply){
    QUrl url = reply->url();
    if(reply->error()){
        printf("Download of %s failed: %s\n",url.toEncoded().constData(),qPrintable(reply->errorString()));
    }else{
        printf("Download of %s succeded!!\n",url.toEncoded().constData());

    }
}
