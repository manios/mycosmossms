#include "stdio.h"
#include "mycosmossms.h"
#include "MycosmosController.h"

// initialise url constants
const QString MyCosmosSms::URL_MYCOSMOS_LOGIN_PAGE = QString("http://mail.mycosmos.gr/mycosmos/login.aspx");
const QString MyCosmosSms::URL_MYCOSMOS_GET_LOST_PASSWORD_PAGE = QString("http://mail.mycosmos.gr/MyCosmos/LostPassword.aspx");
const QString MyCosmosSms::URL_MYCOSMOS_GET_COST_PAGE = QString("http://mail.mycosmos.gr/mycosmos/CostControl.aspx");
const QString MyCosmosSms::URL_MYCOSMOS_SETTINGS_PAGE = QString("http://mail.mycosmos.gr/mycosmos/Settings.asp");
const QString MyCosmosSms::URL_MYCOSMOS_SMS_PAGE = QString("http://mail.mycosmos.gr/mycosmos/SMS_Send.aspx");

MyCosmosSms::MyCosmosSms(QObject *parent) :
    QObject(parent)
{
    contra = new MycosmosController();

    connect(contra,SIGNAL(successfulResponse()),this,SLOT(serverRespondedSuccessfully()));
}

void MyCosmosSms::getLostPassword(QString &telephoneNumber){
    connect(contra,SIGNAL(successfulResponse()),this,SLOT(getLostPasswordPhase1Complete()));
    myTelephone = telephoneNumber;
    contra->getPage(URL_MYCOSMOS_GET_LOST_PASSWORD_PAGE,URL_MYCOSMOS_GET_LOST_PASSWORD_PAGE);
}

int MyCosmosSms::connectToHost(QString &username,QString &password){
    username += "ewfef";

    return 0;
}

int MyCosmosSms::sendMessage(QString &telephoneNumber,QString &messageText){

    return 0;
}

int MyCosmosSms::disconnectFromHost(QString &username,QString &password){

    return 0;
}

bool MyCosmosSms::isConnected(){

    return 0;
}

void MyCosmosSms::serverRespondedSuccessfully(){
    //    contra->postPage( contra->viewStateString,"","");
}

void MyCosmosSms::getLostPasswordPhase1Complete(){
    disconnect(contra,SIGNAL(successfulResponse()),this,SLOT(getLostPasswordPhase1Complete()));
    connect(contra,SIGNAL(successfulResponse()),this,SLOT(getLostPasswordPhase2Complete()));

    // create request data
    QString pData = "__VIEWSTATE=" +QUrl::toPercentEncoding(contra->getViewState(),0,0) +
            +"&txtMobileNo="+QUrl::toPercentEncoding(myTelephone,0,0)
            +"&btnSend=Send";

    // post page with telephone
    contra->postPage(URL_MYCOSMOS_GET_LOST_PASSWORD_PAGE,URL_MYCOSMOS_GET_LOST_PASSWORD_PAGE,pData);
}

void MyCosmosSms::getLostPasswordPhase2Complete(){
    disconnect(contra,SIGNAL(successfulResponse()),this,SLOT(getLostPasswordPhase2Complete()));

    // Error
    printf("Lost password send successfully to: %s",qPrintable(myTelephone));
}
