#include "client.h"

Client::Client(Actor *parent): Actor(parent)
{

}

Client::~Client(){
}

void Client::request(Actor* crawlServer, const QUrl &url, int depth)
{
    // Forward ClientNode petition to crawlserver
    send(crawlServer, "crawlRequest", url, depth);
}

void Client::results(const QUrl& url,const QList<QVariant>& links){
    // Relay response back to clientNode and destroy itself
    Q_EMIT(response(url, links));
    kill();
}
