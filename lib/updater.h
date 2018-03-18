#ifndef UPDATER_H
#define UPDATER_H

#include "updater_global.h"

#include <QtCore/QObject>
#include <QtCore/QVersionNumber>


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
 *      "file": "setup.exe"
 *  }
 *
 * version - the version of the available update
 * file - the file of the update to download. This parameter specifies the path which the file is located at thus telling the Updater from where to download it
 */
class UPDATERSHARED_EXPORT Updater : public QObject
{
    Q_OBJECT

public:
    virtual ~Updater();

    /**
     * @brief checkUpdate
     * checks if an update is available. If so, the "updateAvaialbe" signal will be emitted
     */
    void checkUpdate();

Q_SIGNALS:
    void updateAvailable();

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

private:
    UpdaterPrivate * const d;
};

}   // Updater

#endif // UPDATER_H
