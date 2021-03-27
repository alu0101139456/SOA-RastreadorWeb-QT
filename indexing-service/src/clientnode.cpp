#include "clientnode.h"

ClientNode::ClientNode(CrawlServer *server): server_(server)
{

}

void ClientNode::requestUrl(const QUrl& url, int depth){
    // Create new client actor to manage request
    client_ = ActorManager::spawn<Client>();

    // Hook into client response signal
    connect(client_, &Client::response, this, &ClientNode::response);

    // Make the request
    client_->request(server_, url, depth);
}
