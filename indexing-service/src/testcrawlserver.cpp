#include "testcrawlserver.h"

TestCrawlServer::TestCrawlServer(Actor *parent) :
    Actor(parent)
{
    server_ = spawn<CrawlServer>();
    send(server_, "crawlRequest", QUrl("file:///home/angel/Documentos"), 2);
}

void TestCrawlServer::results(const QUrl& url, const QVariantList &urls)
{
    std::cout << "Recibido resultados de la URLs: " << '\n';
    std::cout << "Numero de URLs obtenidas: " << urls.size();
    std::cout << "Lista:\n";
    for (QVariant url : urls) {
        std::cout << url.toUrl().toString().toStdString() << '\n';
    }
    kill();
}

TestCrawlServer::~TestCrawlServer() = default;
