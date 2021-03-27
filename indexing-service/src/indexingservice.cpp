#include "indexingservice.h"

IndexingService::IndexingService()
{
    // Create a host for clients to register nodes
    clientHost_ = new QRemoteObjectHost(QStringLiteral("local:client"), QStringLiteral("local:registry"));

    // Create crawserver for indexing service instance
    server_ = ActorManager::spawn<CrawlServer>();

    // Get remove notification
    connect(clientHost_, &QRemoteObjectNode::remoteObjectRemoved, this, &IndexingService::objectRemoved);
    connect(clientHost_, &QRemoteObjectNode::remoteObjectAdded, this, &IndexingService::objectAdded);
}

QString IndexingService::request()
{

    // When requested first try to allocate a free node, in case none exist
    //  create a new node and add it to the host
    ClientInfo clientInfo;
    if (!freeClients_.isEmpty()){
        clientInfo = freeClients_.dequeue();

    } else {
        clientInfo.first = "client" + QString::number(clientId_++);
        clientInfo.second = new ClientNode(server_);
        clientHost_->enableRemoting(clientInfo.second, clientInfo.first);
        std::cout << "New Client added, there are " << currentClients_.size() << " Clients on the server\n";
    }
    currentClients_.insert(clientInfo.first, clientInfo.second);

    // Return client name to connect from remote client
    return clientInfo.first;

}

void IndexingService::disconnectClient(QString clientName)
{

    // Enqueue client and remove it from current clients
    ClientInfo oldClient(clientName, currentClients_[clientName]);
    freeClients_.enqueue(oldClient);
    int itemsRemoved = currentClients_.remove(clientName);
    if(itemsRemoved <= 0){
        std::cout << "No clients were removed\n";
    } else {
        std::cout << "Client moved, there currently are ";
        std::cout << currentClients_.size();
        std::cout << " on the service\n";
    }
}

void IndexingService::removeClient(ClientNode * clientNode)
{
    // Disable remote client node
    bool disabled = clientHost_->disableRemoting(clientNode);
    if (!disabled) {
        std::cout << "The object wasnt ClientNode or it wasnt registered\n";
    }
}

void IndexingService::objectRemoved(const QRemoteObjectSourceLocation& loc){
    QString text = loc.first + ": " + loc.second.typeName;
    std::cout << text.toStdString() << " Removed\n";
}

void IndexingService::objectAdded(const QRemoteObjectSourceLocation& loc){
    QString text = loc.first + ": " + loc.second.typeName;
    std::cout << text.toStdString() << " Added\n";
}
