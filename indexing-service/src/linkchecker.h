#ifndef LINKCHECKER_H
#define LINKCHECKER_H

#include <actor.h>
#include <QSet>
#include <QHash>
#include <QUrl>
#include "getter.h"
#include <QQueue>
#include <atomic>

const int MAX_HTTPGETTER_INSTANCES =  200;
const qint64 GETTER_TIMEOUT = 5000;

class TestLinkChecker;

// Actor in charge of exploring url until the desired depth is reached
class LinkChecker : public Actor
{
    Q_OBJECT
public:
    explicit LinkChecker(Actor* actor);
    ~LinkChecker();

public Q_SLOTS:
    // Recieves CrawlServer request to process url at certain depth
    void request(const QUrl& url, int depth);  // From CrawlServer

    // Recieves an url found by a getter with the depth it was found on
    void checkUrl(const QUrl& url, int depth); // From Getter

    // Gets notified that Getter finished processing its url
    void done(); // From Getter

    // Timeout mechanism for Linkchecker
    void timeout(int timerId);

    // Periodically checks to launch remaining Getters for pending URL
    void check_getter_available(int timerId);


private:
    static std::atomic<int> currentTotalGetters_; // Total Getters shared within Linkcheckers

    int currentGetters_ = 0; // Getters in the local Linkchecker

    QSet<QUrl> foundUrls_;
    QSet<QUrl> checkedUrls_;

    QUrl    originalUrl_;
    int     originalDepth_;
    QString originalDomain_;

    // If a url cannot be processed at the moment it inserts it in the queue to
    //  process it at a later time
    QQueue<QPair<QUrl, int>> pendingUrls_;

    int getterAvId_;
    int timeoutId_;

    // Returns domain of the url to compare it with original domain and
    //  decide if the url must be processed
    QString getDomain(const QUrl& url);

    // Checks if url must be processed and deploys new Getter in case it is needed
    void createGetterAndRequest(const QUrl& url, int depth);

    // Populates variant list to send to CrawlServer
    void returnResults();


};

#endif // LINKCHECKER_H
