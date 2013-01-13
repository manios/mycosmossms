#include "stdio.h"
#include "mycosmossms.h"
#include "MycosmosController.h"
#include "QRegExp"

// initialise url constants
const QString MyCosmosSms::URL_MYCOSMOS_LOGIN_PAGE = QString("http://mail.mycosmos.gr/mycosmos/login.aspx");
const QString MyCosmosSms::URL_MYCOSMOS_LOGOUT_PAGE = QString("http://mail.mycosmos.gr/exchweb?cmd=logoff");
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

MyCosmosSms::~MyCosmosSms()
{
    delete contra;
}

void MyCosmosSms::getLostPassword(QString &telephoneNumber){
    connect(contra,SIGNAL(successfulResponse()),this,SLOT(getLostPasswordPhase1Complete()));
    myTelephone = telephoneNumber;
    contra->getPage(URL_MYCOSMOS_GET_LOST_PASSWORD_PAGE,URL_MYCOSMOS_GET_LOST_PASSWORD_PAGE);
}


void MyCosmosSms:: getCost(QString &telephoneNumber,QString &password){
    connect(contra,SIGNAL(successfulResponse()),this,SLOT(getCostPhase1Complete()));

    myTelephone = telephoneNumber;
    myPassword = password;

    // create request data
    QString pData = "__EVENTTARGET=&__EVENTARGUMENT=&tbUsername="+QUrl::toPercentEncoding(myTelephone,0,0)
            +"&tbPassword=" +QUrl::toPercentEncoding(myPassword,0,0)
            + "&btLogin=Log%20On&rbSecurityPub=rbSecurityPub";

    // post page to log in
    contra->postPage(URL_MYCOSMOS_LOGIN_PAGE,URL_MYCOSMOS_LOGIN_PAGE,pData);

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

void MyCosmosSms::getCostPhase1Complete(){
 printf("In phase %d",1);
    disconnect(contra,SIGNAL(successfulResponse()),this,SLOT(getCostPhase1Complete()));
    connect(contra,SIGNAL(successfulResponse()),this,SLOT(getCostPhase2Complete()));

    QString urlMailExhcange = "http://mail.mycosmos.gr/exchange/";
    QString urlMailExhcangeReferer  ="http://mail.mycosmos.gr/mycosmos/MyCosmos.aspx";

    contra->getPage(urlMailExhcange,urlMailExhcangeReferer);
}

void MyCosmosSms::getCostPhase2Complete(){
printf("In phase %d",2);
    disconnect(contra,SIGNAL(successfulResponse()),this,SLOT(getCostPhase2Complete()));
    connect(contra,SIGNAL(successfulResponse()),this,SLOT(getCostPhase3Complete()));

    QString urlSettings =     "http://mail.mycosmos.gr/mycosmos/Settings.asp";
    QString urlSettingsReferer = "http://mail.mycosmos.gr/mycosmos/top.aspx?Theme=Themes/0/1024.css";

    contra->getPage(urlSettings,urlSettingsReferer);
}

void MyCosmosSms::getCostPhase3Complete(){
printf("In phase %d",3);
    disconnect(contra,SIGNAL(successfulResponse()),this,SLOT(getCostPhase3Complete()));
    connect(contra,SIGNAL(successfulResponse()),this,SLOT(getCostPhase4Complete()));

    QString urlCostCont = "http://mail.mycosmos.gr/mycosmos/CostControl.aspx";
    QString urlCostContReferer = "http://mail.mycosmos.gr/mycosmos/toolbar.aspx?DataURL=&Theme=&res=1280&sec=";

    contra->getPage(urlCostCont,urlCostContReferer);
}

void MyCosmosSms::getCostPhase4Complete(){
printf("In phase %d",4);
    disconnect(contra,SIGNAL(successfulResponse()),this,SLOT(getCostPhase4Complete()));
    connect(contra,SIGNAL(successfulResponse()),this,SLOT(getCostPhase5Complete()));

    // get cost from html response
    QRegExp costPattern("^.*Number of SMS Messages sent by Web Site</td>[^<]*<td>([0-9]*)</td>[^<]*<td>10</td>.*$");
    int currCost = contra->getHtmlResponse().replace(costPattern,"\\1").toInt();

    // emit signal with the current cost
    emit costRetrieved(currCost,10);

    // print current cost
    printf("Cost %d/10 for today",currCost);

    // log out
    contra->getPage(URL_MYCOSMOS_LOGOUT_PAGE,URL_MYCOSMOS_GET_COST_PAGE);

}
void MyCosmosSms::getCostPhase5Complete(){
 printf("In phase %d",5);
    // successfull logout

    disconnect(contra,SIGNAL(successfulResponse()),this,SLOT(getCostPhase4Complete()));

    printf("Disconnected successfully!");
}
