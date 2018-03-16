#ifndef UPDATER_P_H
#define UPDATER_P_H

#include <QtCore/QUrl>
#include <QtCore/QVersionNumber>

#include <QtNetwork/QNetworkAccessManager>


namespace Updater {

class UpdaterPrivate
{
public:
    UpdaterPrivate()
        : networkAccessManager(new QNetworkAccessManager)
    {}

    ~UpdaterPrivate()
    {
        delete networkAccessManager;
    }


    QNetworkAccessManager *networkAccessManager;
    QUrl updateServerUrl;
    QVersionNumber currentVersion;
};

}   // Updater


#endif  // UPDATER_P_H
