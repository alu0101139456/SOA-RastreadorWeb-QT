#ifndef CRALWSERVER_H
#define CRALWSERVER_H

#include <actor.h>
#include "linkchecker.h"
#include <map>
#include <QUrl>
#include <iostream>
#include <QPair>
#include <QMap>


// Actor in charge of managing top level client request. It creates Linkcheckers to
//  handle the requests.
class CrawlServer : public Actor
{
    Q_OBJECT
public:
    explicit CrawlServer(Actor* parent);
    ~CrawlServer();


Q_SIGNALS:
    // Signal emmited to notify non-actors that results arrived. Not being used
    void crawlResult(const QList<QVariant>& links);

public Q_SLOTS:
    // Recieves Client request for a url at a certain depth
    void crawlRequest(const QUrl& url, int depth); //Desde cliente

    // Recieves Linkchecker results from a certain request
    //  - url: Is the original url from the request
    //  - requestedDepth: The depth from the request
    //  - links: Url found by Linkchecker
    void result(const QUrl& url, int requestedDepth, const QList<QVariant>& links); //Desde el LinkChecker.

private:
    // Tracks every pending request and the actors that made it
    //  - Url
    //  - Depth
    QMap<QPair<QUrl, int>, QList<Actor*>> checks_;


};

#endif // CRALWSERVER_H
