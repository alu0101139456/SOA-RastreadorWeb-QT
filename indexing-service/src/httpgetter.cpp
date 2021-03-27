#include <QUrl>
#include <QDebug>
#include "httpgetter.h"
#include "schemapluginmanager.h"

// gumbo-query
#include "Document.h"
#include "Node.h"

#include <iostream>

HttpGetter::HttpGetter(Actor* parent)
    : Actor(parent)
{
}

HttpGetter::~HttpGetter() {

}

void HttpGetter::request(const QUrl &url, int depth)
{

//    std::cout << "Processing request: " << url.toString().toStdString() << " with depth: " << depth << '\n';
    auto fetcher = SchemaPluginManager::instance().getSchemaPlugin(url.scheme().toStdString());
    if (! fetcher) {
        reply("done");
        kill();
        return;
    }

    auto info = fetcher.get()->fetchUrl(url.toString().toStdString());

    if( !info || info.value().contentType.find("text/html",0) == std::string::npos) {
        reply("done");
        kill();
        return;
    }

    CDocument doc;
    doc.parse(info.value().content);
    CSelection elements = doc.find("[href], [src]");
    for(size_t i =0; i < elements.nodeNum(); ++i) {
        auto node = elements.nodeAt(i);
        auto hrefAttribute = node.attribute("href");
        auto srcAttribute = node.attribute("stc");
        if( !hrefAttribute.empty()) {
            auto urlFound = url.resolved(QString::fromStdString(hrefAttribute));
            reply("checkUrl", urlFound, depth);
//            qDebug() << "HttpGetter: URL encontrada " << url.resolved(QString::fromStdString(hrefAttribute));
        }
        if( !srcAttribute.empty()) {
            auto urlFound = url.resolved(QString::fromStdString(srcAttribute));
            reply("checkUrl", urlFound, depth);
//            qDebug() << "HttpGetter: URL encontrada " << url.resolved(QString::fromStdString(srcAttribute));
        }

    }

    reply("done");
    kill();

}
