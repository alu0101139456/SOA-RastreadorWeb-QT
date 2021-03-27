#ifndef GETTER_H
#define GETTER_H

#include <actor.h>
#include "schemapluginmanager.h"
// gumbo-query
#include "Document.h"
#include "Node.h"
#include <QUrl>
#include <QIODevice>

// Actor in charge of processign a single url. It relays back all the urls found within
//  the petition
class Getter : public Actor
{
    Q_OBJECT
public:
    explicit Getter(Actor* parent);
    ~Getter();

public Q_SLOTS:
    // Recieves Linkchecker request to explore url. Depth is used to relay back the
    //  urls found with the new depth = depth - 1
    void request(const QUrl& url, int depth); //Desde el Linkcheker

Q_SIGNALS:
    // Notifies Linkchecker that an url was found at a certain depth level
    void checkUrl (const QUrl& url, int depth);

};

#endif // GETTER_H
