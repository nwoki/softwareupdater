#ifndef UPDATER_H
#define UPDATER_H

#include "updater_global.h"

#include <QtCore/QObject>
#include <QtCore/QVersionNumber>

#include <QtNetwork/QNetworkReply>

namespace SoftwareUpdater {

class UpdaterPrivate;


/**
 * @class Updater
 * @author Francesco Nwokeka <francesco.nwoekeka@gmail.com>
 *
 * The base class for the updater library. It handles the downloads and notifications for the new version download and
 * update status.
 *
 * The update file is a JSON file structured in the following way:
 *
 *  {
 *      "version": "x.x.x"
 *      "file_path": "setup.exe"
 *  }
 *
 * version - the version of the available update
 * file - the file of the update to download. This parameter specifies the path which the file is located at thus telling the Updater from where to download it
 */
class UPDATERSHARED_EXPORT Updater : public QObject
{
    Q_OBJECT

    Q_PROPERTY (DownloadState downloadState     READ downloadState      NOTIFY downloadStateChanged)

public:
    struct UpdateFileData {
        QVersionNumber version = QVersionNumber::fromString("0.0.0");
        QString downloadDir;
        QString downloadFile;
    };

    enum DownloadState {
        IDLE,
        DOWNLOADING,
        COMPLETED,
        ERROR
    };

    virtual ~Updater();

    /**
     * @brief downloadedFilePath
     *
     * Path to the downloaded software file
     */
    QString downloadedFilePath() const;

    /**
     * @brief downloadstate
     *
     * returns the info on the current download state
     */
    DownloadState downloadState() const;

    /**
     * @brief downloadupdate
     *
     * Starts the download of the update file if available. If no download is available, and the function is call, an error will be sent via the
     * appropriate signal. Call this after having received an "updateAvailable" signal and you want to download the new file
     */
    void downloadUpdate();


public Q_SLOTS:
    /**
     * @brief checkUpdate
     * checks if an update is available. If so, the "updateAvaialbe" signal will be emitted
     */
    void checkUpdate();

Q_SIGNALS:
    /**
     * @brief downloadProgressUpdate
     * @param downloaded bytes downloaded
     * @param totalToDownload total bytes to download
     *
     * notifies the download progress
     */
    void downloadProgressUpdate(quint64 downloaded, quint64 totalToDownload);

    /**
     * @brief downloadStateChanged
     * @param state the state of the current download
     *
     * notifies the state of the downloads
     */
    void downloadStateChanged(DownloadState state);

    /** notifies the user that a newer version is available to download */
    void updateAvailable(const QVersionNumber &version);

protected:
    /**
     * @brief Updater
     * @param the update server url (http) from where to check for downloads
     * @param currentVersion the current application version to check against
     * @param parent
     */
    Updater(const QString &updateServerUrl
            , const QVersionNumber &currentVersion
            , QObject *parent = nullptr);

    /**
     * @brief updateReady
     *
     * this method is to be implemented by the developer in order to act upon the downloaded update file
     */
    virtual void updateReady() = 0;

private Q_SLOTS:
    void onCheckUpdateReceived();
    void onRequestError(QNetworkReply::NetworkError error);

private:
    void setDownloadState(DownloadState state);

    UpdaterPrivate * const d;
};

}   // Updater

#endif // UPDATER_H
