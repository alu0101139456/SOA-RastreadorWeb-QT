#ifndef CLIENTNODE_H
#define CLIENTNODE_H

#include <rep_clientnode_source.h>

#include <actor.h>
#include "crawlserver.h"
#include "client.h"

// Connection point between client and server
// Creates actors that take care of making request to stop crawlserver
//   from broadcasting the answer
class ClientNode : public ClientNodeSimpleSource
{
    Q_OBJECT
public:
    ClientNode(CrawlServer* server);

public Q_SLOTS:
    // Interface to remote client
    void requestUrl(const QUrl& url, int depth);

private:

    Client* client_;        // Actor created for the request
    CrawlServer* server_;
};

#endif // CLIENTNODE_H
