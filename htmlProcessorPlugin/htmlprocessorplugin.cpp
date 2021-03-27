#include "htmlprocessorplugin.h"
#include <iostream>

HtmlProcessorPlugin::HtmlProcessorPlugin()
{
}

QList<QUrl> HtmlProcessorPlugin::extractUrls(const QUrl &url, QIODevice *type)
{
    // Declaring needed data structures
    CDocument doc;
    QList<QUrl> result;

    // Return to beginning of the stream
    type->seek(0);

    // Parse info on stream to doc
    doc.parse(type->readAll().data());
    type->close();

    // Get all link tags
    CSelection elements = doc.find("[href], [src]");
    for(size_t i =0; i < elements.nodeNum(); ++i) {
        auto node = elements.nodeAt(i);
        auto hrefAttribute = node.attribute("href");
        auto srcAttribute = node.attribute("src");

        // If link tag is not empty store it
        if( !hrefAttribute.empty()) {
            auto urlFound = url.resolved(QString::fromStdString(hrefAttribute));
            result.push_back(urlFound);
        }
        if( !srcAttribute.empty()) {
            auto urlFound = url.resolved(QString::fromStdString(srcAttribute));
            result.push_back(urlFound);
        }

    }

    return result;
}

std::vector<std::string> HtmlProcessorPlugin::fileSupported()
{
    static std::vector<std::string> filetype = {
        "text/html; charset=UTF-8",
        "text/html"
    };
    return filetype;
}

SchemaPlugin* loadPlugin() {
    return new HtmlProcessorPlugin;
}
