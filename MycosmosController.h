#ifndef MYCOSMOSCONTROLLER_H
#define MYCOSMOSCONTROLLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>


class mycosmoscontroller : public QObject
{
    Q_OBJECT
public:
   mycosmoscontroller(QObject *parent = 0);
   void downloadPage();
signals:
    
public slots:
   void downloadFinished(QNetworkReply *reply);
private:
   QUrl url;
   QNetworkAccessManager nManager;
   QNetworkReply *nReply;
};

#endif // MYCOSMOSCONTROLLER_H
