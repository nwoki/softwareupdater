#ifndef UPDATER_P_H
#define UPDATER_P_H

#include "updater.h"
#include "updater_global.h"

#include <QtCore/QFile>
#include <QtCore/QUrl>

#include <QtNetwork/QNetworkAccessManager>


namespace SoftwareUpdater {

class UPDATERSHARED_EXPORT UpdaterPrivate
{

public:
    UpdaterPrivate()
        : networkAccessManager(new QNetworkAccessManager)
        , downloadedFile(new QFile)
        , downloadState(Updater::IDLE)
    {
        // default current version
        currentVersion = QVersionNumber::fromString("0.0.0", nullptr);
    }

    ~UpdaterPrivate()
    {
        delete networkAccessManager;

        if (downloadedFile->isOpen()) {
            downloadedFile->close();
        }

        delete downloadedFile;
    }


    QNetworkAccessManager *networkAccessManager;
    QUrl updateServerUrl;
    QVersionNumber currentVersion;

    Updater::UpdateFileData updateFileData;

    /** the QFile that we'll use for downloading the software updates */
    QFile *downloadedFile;

    Updater::DownloadState downloadState;
};

}   // SoftwareUpdater


#endif  // UPDATER_P_H
