#ifndef TESTCRAWLSERVER_H
#define TESTCRAWLSERVER_H

#include <QObject>
#include "crawlserver.h"
#include <iostream>

class TestCrawlServer : public Actor
{
    Q_OBJECT
public:
    explicit TestCrawlServer (Actor* parent = nullptr);
    ~TestCrawlServer();

public Q_SLOTS:
    void results(const QUrl& url, const QVariantList &urls); // Desde el crawlServer

private:
    CrawlServer* server_;
};

#endif // TESTCRAWLSERVER_H
