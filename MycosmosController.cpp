#include "MycosmosController.h"
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QDebug>
#include <QRegExp>
#include <QList>
#include <stdio.h>


MycosmosController::MycosmosController(QObject *parent) :
    QObject(parent)
{
    nManager = new QNetworkAccessManager(this);
    cookieData = QVariant();

    connect(nManager,SIGNAL(finished(QNetworkReply*)),SLOT(downloadFinished(QNetworkReply*)));
}

MycosmosController::~MycosmosController(){
    delete nReply;
    delete nManager;
}

void MycosmosController::downloadPage(){
    //    QString urlString = "http://www.google.gr";
    QString urlString = "http://mail.mycosmos.gr/mycosmos/login.aspx";

    //        QUrl urliki(urlString);
    //        QString refererUrl = NULL;

    //        QNetworkRequest nRequest(urlString);
    //        addRequestHeaders(nRequest,refererUrl);

    //        nManager->get(nRequest);
    getPage(urlString,urlString);

}

void MycosmosController::getPage(const QString &pageUrl,const QString &refererUrl){
    QNetworkRequest nRequest(pageUrl);

    // add request headers
    addRequestHeaders(nRequest,refererUrl);

    // add cookies
    if(!cookieData.isNull()){
        nRequest.setHeader(QNetworkRequest::CookieHeader, cookieData);
    }

    // make a GET request
    nManager->get(nRequest);

}

void MycosmosController::postPage(const QString &pageUrl,const QString &refererUrl,const QString &params){
    QNetworkRequest nRequest(pageUrl);

    // add request headers
    addRequestHeaders(nRequest,refererUrl);

    // add cookies
    if(!cookieData.isNull()){
        nRequest.setHeader(QNetworkRequest::CookieHeader, cookieData);
    }

    // UrlEncode request data and make a POST request
    nManager->post(nRequest,params.toUtf8());

}

void MycosmosController::downloadFinished(QNetworkReply *reply){
    QUrl url = reply->url();

    if(reply->error() == QNetworkReply::NoError) {
        // Get the http status code
        int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();


        if (v >= 200 && v < 300){ // Success

        }
        else if (v >= 300 && v < 400) {// Redirection

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

        // Here we got the final reply
        htmlReplyText = QString::fromUtf8(reply->readAll());

        // get view state string
        viewStateString =  getViewState(htmlReplyText);

        QList<QNetworkCookie>  cookies = nManager->cookieJar()->cookiesForUrl(url);
        for(int i=0; i<cookies.count(); i++){
            printf("COOKIE %d Name:%s Value:%s\n" ,i,qPrintable(QString::fromUtf8(cookies.at(i).name())),qPrintable(QString::fromUtf8(cookies.at(i).value())) );
        }
        printf("COOKIES for %s\n%s\n" ,qPrintable(url.host()) ,qPrintable(QString::number(cookies.count())));

        // save cookies inside a QVariant
        cookieData.setValue(cookies);

        emit successfulResponse();
    }
    else
    {
        // Error
        printf("Download of %s failed: %s\n",url.toEncoded().constData(),qPrintable(reply->errorString()));
    }

    printf("Url encoded Νένα 1:%s\n" ,qPrintable(QUrl::toPercentEncoding("Νένα 1",0,0)));


    // We receive ownership of the reply object
    // and therefore need to handle deletion.
    //    delete reply;
}

QString MycosmosController::getViewState(QString &htmlResponse){
    return htmlResponse.replace(QRegExp("^.*__VIEWSTATE\" value=\"([^\"]*)\".*$"),"\\1");
}

QVariant MycosmosController::getCookieData(){
    return cookieData;
}

QString MycosmosController::getHtmlResponse(){
    return htmlReplyText;
}

QString MycosmosController::getViewState(){
    return viewStateString;
}
void MycosmosController::addRequestHeaders(QNetworkRequest &requesti,const QString &referer){
    requesti.setRawHeader("User-Agent","Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:18.0) Gecko/20100101");
    requesti.setRawHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    requesti.setRawHeader("Accept-Language","en-US,en;q=0.5");
    requesti.setRawHeader("Accept-Encoding", "gzip, deflate");
    requesti.setRawHeader("Accept-Charset","ISO-8859-7,utf-8;q=0.7,*;q=0.7");

    requesti.setRawHeader("Content-Type","application/x-www-form-urlencoded");

    if (!referer.isNull() && !referer.isEmpty()) {
        requesti.setRawHeader("Referer", referer.toUtf8());
    }
}
