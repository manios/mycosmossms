#ifndef MYCOSMOSSMS_H
#define MYCOSMOSSMS_H

#include "MycosmosController.h"
#include <QObject>
#include <QString>

class MyCosmosSms : public QObject
{
    Q_OBJECT
public:
    MyCosmosSms(QObject *parent = 0);
    void getLostPassword(QString &telephoneNumber);
    int connectToHost(QString &username,QString &password);
    int sendMessage(QString &telephoneNumber,QString &messageText);
    int disconnectFromHost(QString &username,QString &password);
    bool isConnected();

signals:
    void lostPasswordSent();
    void smsSent();
    void costRetrieved(int currentCost,int totalCost);

private:
    MycosmosController *contra;
    static const QString URL_MYCOSMOS_LOGIN_PAGE;
    static const QString URL_MYCOSMOS_GET_LOST_PASSWORD_PAGE;
    static const QString URL_MYCOSMOS_GET_COST_PAGE;
    static const QString URL_MYCOSMOS_SETTINGS_PAGE;
    static const QString URL_MYCOSMOS_SMS_PAGE;

    QString myTelephone;
    QString myPassword;
private slots:
    void serverRespondedSuccessfully();
    void getLostPasswordPhase1Complete();
    void getLostPasswordPhase2Complete();
};

#endif // MYCOSMOSSMS_H
