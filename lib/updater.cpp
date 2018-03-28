#include "updater.h"
#include "updater_p.h"

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonParseError>

#include <QtNetwork/QNetworkRequest>

namespace SoftwareUpdater {

Updater::Updater(const QString &updateServerUrl
                 , const QVersionNumber &currentVersion
                 , QObject *parent)
    : QObject(parent)
    , m_downloadState(IDLE)
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

QString Updater::downloadedFilePath() const
{
    return d->downloadedFile->fileName();
}

SoftwareUpdater::Updater::DownloadState Updater::downloadState() const
{
    return m_downloadState;
}

void Updater::downloadUpdate()
{
    // download file setup
    d->downloadedFile->setFileName(QDir::tempPath() + QDir::separator() + d->updateFileData.downloadFile);

    qDebug() << "CREATED FILE TO DOWNLOAD IN: " << d->downloadedFile->fileName();

    if (!d->downloadedFile->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning("[Updater::downloadUpdate] problem with temp file: %s", qUtf8Printable(d->downloadedFile->errorString()));
        return;
    }

    // prepare the download url
    QUrl downloadUrl(d->updateServerUrl.toString() + d->updateFileData.downloadDir + "/" + d->updateFileData.downloadFile);

    // TODO setup access user:password if needed

    QNetworkReply *reply = d->networkAccessManager->get(QNetworkRequest(downloadUrl));

    // update the downloading state
    setDownloadState(DOWNLOADING);

    connect(reply, &QNetworkReply::readyRead, [this, reply] () {
        d->downloadedFile->write(reply->readAll());
    });

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onRequestError(QNetworkReply::NetworkError)));

    connect(reply, &QNetworkReply::finished, [this, reply] () {
        d->downloadedFile->flush();
        d->downloadedFile->close();

        setDownloadState(COMPLETED);

        delete reply;

        // call the virtual function so that the developer's implementation acts upon the update file
        updateReady();
    });
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
        // store the update info for when we'll want to download the file
        d->updateFileData.version = QVersionNumber::fromString(jsonMap["version"].toString());
        d->updateFileData.downloadDir = jsonMap["dir"].toString();
        d->updateFileData.downloadFile = jsonMap["file"].toString();

        Q_EMIT updateAvailable();
    }

    delete reply;
}

void Updater::onRequestError(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error);

    qDebug("[Updater::onRequestError]");
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (!reply) {
        return;
    }

    qDebug() << reply->errorString();
    delete reply;
}

void Updater::setDownloadState(SoftwareUpdater::Updater::DownloadState state)
{
    m_downloadState = state;
    Q_EMIT downloadStateChanged();
}


}   // Updater
