#include "updater.h"
#include "updater_p.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonParseError>

#include <QtNetwork/QNetworkRequest>

namespace SoftwareUpdater {

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
    // pull the update file from the server
    //
    // keep the network request seperate in case we need to setup some configuration options later
    // on in the evolution of the lib

    QNetworkRequest updateReq(d->updateServerUrl);
    QNetworkReply *updateReqReply = d->networkAccessManager->get(updateReq);

    connect(updateReqReply, &QNetworkReply::finished, this, &Updater::onCheckUpdateReceived);
    connect(updateReqReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onRequestError(QNetworkReply::NetworkError)));
}

void Updater::onCheckUpdateReceived()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (!reply) {
        return;
    }

    QJsonParseError jsonParseError;

    // check the update json file
    QVariantMap jsonMap = QJsonDocument::fromJson(reply->readAll(), &jsonParseError).toVariant().toMap();

    if (jsonParseError.error != QJsonParseError::NoError) {
        qWarning("[Updater::onCheckUpdateReceived] error with downloaded json: %s", qUtf8Printable(jsonParseError.errorString()));
        return;
    }

    if (QVersionNumber::compare(QVersionNumber::fromString(jsonMap["version"].toString()), d->currentVersion) > 0) {
        Q_EMIT updateAvailable();
    }

    delete reply;
}


}   // Updater
