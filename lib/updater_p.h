#ifndef UPDATER_P_H
#define UPDATER_P_H

#include <QtCore/QFile>
#include <QtCore/QUrl>
#include <QtCore/QVersionNumber>

#include <QtNetwork/QNetworkAccessManager>


namespace SoftwareUpdater {

class UpdaterPrivate
{
public:
    struct UpdateFileData {
        QVersionNumber version = QVersionNumber::fromString("0.0.0");
        QString downloadDir;
        QString downloadFile;
    };


    UpdaterPrivate()
        : networkAccessManager(new QNetworkAccessManager)
        , downloadedFile(new QFile)
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

    UpdateFileData updateFileData;

    /** the QFile that we'll use for downloading the software updates */
    QFile *downloadedFile;
};

}   // SoftwareUpdater


#endif  // UPDATER_P_H
