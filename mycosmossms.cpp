#include "mycosmossms.h"
#include "MycosmosController.h"


static const QString MyCosmosSms::URL_MYCOSMOS_LOGIN_PAGE = QString("http://mail.mycosmos.gr/mycosmos/login.aspx");

MyCosmosSms::MyCosmosSms(QObject *parent) :
    QObject(parent)
{
    contra = new MycosmosController();

    connect(contra,SIGNAL(successfulResponse()),this,SLOT(serverRespondedSuccessfully()));
}

void MyCosmosSms::getLostPassword(QString &telephoneNumber){

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

}
