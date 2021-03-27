#include <QMessageBox>
#include <QRemoteObjectNode>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#define N_EXT "2"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , remoteObjectNode_(new QRemoteObjectNode(REMOTE_OBJECT_REGISTRY, this))
{
    statusMessage_ = "";
    // Import resource (loading gif)
    Q_INIT_RESOURCE(resource);
    ui->setupUi(this);
    updateStatusMessage("Not connected");
    loadingIcon_ = new QLabel(this);

    // Prepare gif and hide it to be used when necessary
    QMovie *movie = new QMovie(":/rec/gif/loader.gif", QByteArray(), loadingIcon_);
    loadingIcon_->setMovie(movie);
    ui->statusbar->addPermanentWidget(loadingIcon_);
    urlType_ = URL_TYPES[0];
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendButton_clicked()
{
    // To be able to send a request we must be connected to indexing service and not have any current clients
    if (!indexingService_.isNull() && client_.isNull()) {

        // Request Indexing service a new client
        pendingReply_ = indexingService_->request();

        // Set watcher for the pending reply and wait until indexing service has given us a valid client name
        QRemoteObjectPendingCallWatcher pendingReplyWatcher(pendingReply_);
        bool connectionSuccessful = waitForNBRequest(&pendingReplyWatcher,
                                                     "finished(QRemoteObjectPendingCallWatcher *)",
                                                     CONNECTION_TIMEOUT);

        if(connectionSuccessful){
            // We have the clientname
            clientName_ = pendingReply_.returnValue();

            // Connect the client to the client node
            client_.reset(remoteObjectNode_->acquire<ClientNodeReplica>(clientName_));
            connectionSuccessful = waitForNBRequest(client_.data(),
                                                    "initialized()",
                                                    CONNECTION_TIMEOUT);


            if (connectionSuccessful)
            {
                // Connect this client to local client response
                connect(client_.get(), &ClientNodeReplica::response, this, &MainWindow::updateResults);
                QString requestedUrl = urlType_ + ui->urlEdit->text();
                ui->results->append(requestedUrl);

                // Send request to ClientNode for given url and depth in textbox and dpthBox
                client_->requestUrl(urlType_ + ui->urlEdit->text(), ui->depthBox->value());
                connect(client_.data(), &ClientNodeReplica::destroyed, this, &MainWindow::destroyClient);

                // Disable window while we are waiting and show it is not frozen by using the loadign gif
                setEnabled(false);
                loadingIcon_->show();
                loadingIcon_->movie()->start();
            } else {
                client_.take();
                QMessageBox::warning(this,
                                     tr("Indexing Client"),
                                     tr("Couldnt connect to client node."),
                                     QMessageBox::Ok);
            }
        } else {
            // timeout and warning
            QMessageBox::warning(this,
                                 tr("Indexing Client"),
                                 tr("Timeout trying to acquire a new client"),
                                 QMessageBox::Ok);
        }
    } else {
        QMessageBox::warning(this,
                             tr("Indexing Client"),
                             tr("Not connected to indexing service"),
                             QMessageBox::Ok);
    }

}

void MainWindow::updateResults (const QUrl& url, const QList<QVariant>& links)
{

    if(!client_.isNull()){
        updateStatusMessage("Connected to indexing service");

        // Append each link in the result text box
        foreach(QVariant variant, links){
            ui->results->append(variant.value<QUrl>().toString());
        }
        loadingIcon_->hide();
        loadingIcon_->movie()->stop();


        // Disconnect and reset client. Then enable window again
        disconnect(client_.data(), &ClientNodeReplica::response, this, &MainWindow::updateResults);
        client_.reset();
        setEnabled(true);


    }
}

void MainWindow::on_connectButton_clicked()
{
    connectToService();
}

void MainWindow::on_disconnectButton_clicked(){

    // Reset all connections
    clientName_ = "";
    client_.reset();
    indexingService_.reset();
    updateStatusMessage("Not connected");

}

void MainWindow::destroyClient()
{

    // Create timer to wait for some time before disconnecting the client to allow service to remove it
    QTimer timeToDelete;
    timeToDelete.setSingleShot(true);
    QEventLoop eventLoop;

    connect(&timeToDelete, &QTimer::timeout, &eventLoop, &QEventLoop::quit);
    timeToDelete.start(1000);
    eventLoop.exec();

    indexingService_->disconnectClient(clientName_);
    clientName_ = "";
}

bool MainWindow::waitForNBRequest(const QObject* sender,
                                  std::string signal,
                                  const int timeout){

    updateStatusMessage("Processign request. Please wait");

    // Set a new timer to cancel request in timeout
    QTimer expireTimer;
    expireTimer.setSingleShot(true);  // Timer should only signal once

    // Create an eventLoop to block input to main window but allow for window refresh
    QEventLoop eventLoop;

    std::string signalText = "2" + signal;
    setEnabled(false);

    // Connect signals to eventLoop. Event loop will end if wether expected signal is raised
    //  or timeout is reached
    connect(&expireTimer, &QTimer::timeout, &eventLoop, &QEventLoop::quit);
    connect(sender, signalText.c_str(), &eventLoop, SLOT(quit()));
    expireTimer.start(timeout);
    eventLoop.exec();


    // Check why loop was ended. If timer is still active then signal must have been
    //  triggered
    bool taskCompleted = false;
    if(expireTimer.isActive()){
        taskCompleted = true;
    }
    // Restore window
    setEnabled(true);
    restoreStatusMessage();

    return taskCompleted;

}

void MainWindow::on_clearButton_clicked()
{
    ui->results->clear();
}

void::MainWindow::connectToService(){
    bool connectionSuccessful;

    // Check if we are already connected to registry
    if (remoteObjectNode_->registry()->state() != QRemoteObjectReplica::Valid)
    {
        // Remove current invalid connection
        delete remoteObjectNode_;

        // Request for new connection to registry
        remoteObjectNode_ = new QRemoteObjectNode(REMOTE_OBJECT_REGISTRY, this);
        connectionSuccessful = waitForNBRequest(remoteObjectNode_->registry(),
                                               "initialized()",
                                               CONNECTION_TIMEOUT);

        // If client connects to registry notify in the status bar
        // IOC send user a warning dialog and exit connection
        if(connectionSuccessful){
            updateStatusMessage("Connected to Registry.");
        } else {
            QMessageBox::warning(this,
                                 tr("Indexing Client"),
                                 tr("Couldnt connect to registry."),
                                 QMessageBox::Ok);
            return;
       }
    }

    // We are now connected to registry. Check if we are already connected to indexing service.
    //  if we are already connected to it notify user with a dialog.
    if (indexingService_.isNull()) {

        // Reset replica to try to connect again
        indexingService_.reset(remoteObjectNode_->acquire<IndexingServiceReplica>());
        connectionSuccessful = waitForNBRequest(indexingService_.data(),
                                        "initialized()",
                                        CONNECTION_TIMEOUT);

        // If client connects to registry notify in the status bar
        // IOC send user a warning dialog and exit connection
        if(connectionSuccessful)
        {
            updateStatusMessage("Connected to Indexing Service");
        } else {
            indexingService_.reset();
            QMessageBox::warning(this, tr("Indexing Client"),
                                       tr("Couldnt connect to indexing service."),
                                       QMessageBox::Ok);
        }
    } else {
        QMessageBox::warning(this, tr("Indexing Client"),
                                   tr("Already connected to a server."),
                                   QMessageBox::Ok);
    }


}

void MainWindow::updateStatusMessage(const QString& newMessage)
{
    // Change actual message, and store it in case we want to return to it
    lastStatusMessage_ = statusMessage_;
    statusMessage_ = newMessage;
    ui->statusbar->showMessage(statusMessage_);
}

void MainWindow::restoreStatusMessage()
{
    // Show last status message
    QString swap = lastStatusMessage_;
    lastStatusMessage_ = statusMessage_;
    statusMessage_ = swap;
    ui->statusbar->showMessage(statusMessage_);
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    // Change url type to currently selected one
    if(index >= 0 && index < N_URL_TYPES)
        urlType_ = URL_TYPES[index];
}
