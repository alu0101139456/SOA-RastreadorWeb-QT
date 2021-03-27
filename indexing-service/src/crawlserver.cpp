#include "crawlserver.h"
#include "getter.h"

CrawlServer::CrawlServer(Actor* parent)
    : Actor(parent)
{

}

CrawlServer::~CrawlServer() {
    std::cout << "SOY EL DESTRUCTOR DEL CRAWLSERVER\n";
}

void CrawlServer::crawlRequest(const QUrl &url, int depth)
{
    // If url depth pair is not in currently being processed append it to receive the response
    // IOC create a new linkchecker and formulate the request
    QPair<QUrl, int> key(url, depth);
    if (checks_.find(key) != checks_.end()) {
        checks_[key].append(sender());
    } else {
        LinkChecker* checker = ActorManager::spawn<LinkChecker>(this);
        send(checker, "request", url, depth);
        QList<Actor*> list; list.append(sender());
        checks_.insert(key, list);
    }
    // Save client data
}

void CrawlServer::result(const QUrl &url, int requestedDepth, const QList<QVariant> &links)
{
    //TODO: Responder a cada cliente pendiente de la petición con la lista de enlaces.
    // Message crawlResponse(url, links)

    // For each client hooked on the request send them the results
    QPair<QUrl, int> key(url, requestedDepth);
    for (Actor* actor: checks_.value(key)) {
        send(actor, "results", url, links);
    }

    // Remove key from urls being processed
    checks_.remove(key);
}
