#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRemoteObjectNode>

//test
#include <QDialog>
#include <QTimer>
#include <QElapsedTimer>
#include <QGraphicsColorizeEffect>
#include <QLabel>
#include <QMovie>
#include <QPixmap>
#include <QBitmap>

#include <rep_indexingservice_replica.h>
#include <rep_clientnode_replica.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum urlType { file, http, https};

const int N_URL_TYPES = 3;
const QString URL_TYPES[N_URL_TYPES] = {"file://", "http://", "https://"}; // For comboBx usage

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Remote object registry url
    const QString REMOTE_OBJECT_REGISTRY = QStringLiteral("local:registry");

    // Timeouts for tasks
    const int CONNECTION_TIMEOUT = 5000;
    const qint64 TMAX = 150;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    // Manages internal events
    void updateResults(const QUrl& url, const QList<QVariant> &links);
    void destroyClient();

    // Manages ui elements
    void on_sendButton_clicked();
    void on_connectButton_clicked();
    void on_disconnectButton_clicked();
    void on_clearButton_clicked();
    void on_comboBox_currentIndexChanged(int index);



Q_SIGNALS:
    // Notifies when indexing service supplies client
    void finishedClientRequest();

    // Notifies indexing service that client is no longer needed
    void clientDestroyed();

private:

    bool registryAvailable_ = false;  // Flag to know if we need yet to connect to registry
    Ui::MainWindow *ui;

    // Remote objects for registry and indexing service
    QRemoteObjectNode* remoteObjectNode_;
    QRemoteObjectNode* remoteIndexingService_;

    // Remote replica of indexing service and client
    QScopedPointer<IndexingServiceReplica, QScopedPointerDeleteLater> indexingService_;
    QScopedPointer<ClientNodeReplica, QScopedPointerDeleteLater> client_;

    // Holds client id from Indexing service reply to request
    QRemoteObjectPendingReply<QString> pendingReply_;

    // Name used to connect to client node
    QString clientName_;

    // Utils for showing window status
    QString lastStatusMessage_;
    QString statusMessage_;
    QLabel  *loadingIcon_;

    // file:// or http:// or https://
    QString urlType_;

    // Non blocking wait for connection to registry and indexing service and client
    bool waitForNBConnection(const QRemoteObjectReplica* replica, std::string loadingMsg, std::string errorMsg);

    // Non blocking wait for indexing service reply
    bool waitForNBRequest(const QObject* sender,
                          std::string signal,
                          const int timeout);


    void connectToService();

    // Tools to mofy status message bar
    void updateStatusMessage(const QString& newMessage);
    void restoreStatusMessage();

};
#endif // MAINWINDOW_H
