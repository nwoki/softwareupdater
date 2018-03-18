#ifndef UPDATER_P_H
#define UPDATER_P_H

#include <QtCore/QUrl>
#include <QtCore/QVersionNumber>

#include <QtNetwork/QNetworkAccessManager>


namespace SoftwareUpdater {

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

}   // SoftwareUpdater


#endif  // UPDATER_P_H
