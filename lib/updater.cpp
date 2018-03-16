#include "updater.h"
#include "updater_p.h"

namespace Updater {

Updater::Updater(const QString &updateServerUrl
                 , const QVersionNumber &currentVersion
                 , QObject *parent)
    : QObject(parent)
    , d(new UpdaterPrivate)
{
    // check for valid url
    d->updateServerUrl = QUrl(updateServerUrl);
    d->currentVersion = currentVersion;

    if (!d->updateServerUrl.isValid()) {
        qWarning("[Updater::Updater] Invalid URL: %s", qUtf8Printable(d->updateServerUrl.toString()));
    }
}

Updater::~Updater()
{
    delete d;
}

void Updater::checkUpdate()
{
    // TODO download update json file from server and notify if update is available
}

}
