#ifndef CLIENT_H
#define CLIENT_H

#include <actor.h>

// Represents a single instance of a remote client within actor class. This way we exploit actor usage by having crawlserver
// respond to actors and them relaying the info back to the client instead of broadcasting the responses
class Client : public Actor
{
    Q_OBJECT
public:
    explicit Client(Actor * parent);
    ~Client();

    // Interface to Client node to create a new request to the crawlserver
    void request(Actor* crawlServer, const QUrl& url, int depth);

public Q_SLOTS:
    // Slot that recieves the results from crawlserver
    void results(const QUrl& url, const QList<QVariant>& links);

Q_SIGNALS:
    // Signal emmited to ClientNode to notify when results are available
    void response(const QUrl& url, const QList<QVariant>& links);

};

#endif // CLIENT_H
