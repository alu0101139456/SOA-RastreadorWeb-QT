#ifndef INDEXINGSERVICE_H
#define INDEXINGSERVICE_H

#include <rep_indexingservice_source.h>
//#include "httpgetter.h"
#include "crawlserver.h"
#include "clientnode.h"

const auto REMOTE_CLIENT_NAME = QStringLiteral("local:indexing-client");
const int MAX_CLIENTS = 10;

// Local source for remote object access
class IndexingService : public IndexingServiceSimpleSource
{
    Q_OBJECT

public:



    IndexingService();

    // Provides local client node for the client to connect
    virtual QString request () override;

    // Disconnects the client node once the request is satisfied
    virtual void disconnectClient(QString clientName) override;


public Q_SLOTS:

    // Notifies when an object is added or removed from the registry
    void objectRemoved(const QRemoteObjectSourceLocation& loc); //Temporal
    void objectAdded(const QRemoteObjectSourceLocation& loc); //Temporal

private:

    // Associates clientname with local node
    using ClientInfo = QPair<QString, ClientNode*>;


    CrawlServer* server_;

    int clientId_ = 0;              // Used when creating new node
    QRemoteObjectHost* clientHost_; // Manages client nodes created

    QHash<QString, ClientNode*> currentClients_;        // Clients reserved
    QQueue<QPair<QString, ClientNode*>> freeClients_;   // Clients to reassign

    //Not using this one yet. As we dont remove clients from queue, we reutilize them
    void removeClient(ClientNode* clientNode);



};

#endif // INDEXINGSERVICE_H
