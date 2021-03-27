

#include "getter.h"


Getter::Getter(Actor* parent)
    : Actor(parent)
{
}

Getter::~Getter() {

}

void Getter::request(const QUrl &url, int depth)
{
    // Get the appropiate schema plugin fetcher or exit in case none is available for current schema
    auto fetcher = SchemaPluginManager::instance().getSchemaPlugin(url.scheme().toStdString());
    if (! fetcher) {
        reply("done");
        kill();
        return;
    }

    // Fetch info from url. Exit n case fecther could not fetch the info
    auto info = fetcher.get()->fetchUrl(url.toString().toStdString());
    if( !info ) {
        reply("done");
        kill();
        return;
    }

    // Get the appropiate processor plugin. Exit in case none is available for current file
    auto processor = SchemaPluginManager::instance().getProcessorPlugin(info.value().contentType);
    if (!processor) {
        if(info->content->isOpen()) info->content->close();
        reply("done");
        kill();
        return;
    }

    // Parse and get urls from buffer
    auto urlList = processor.get()->extractUrls(url, info.value().content);

    // Relay found urls back to requester
    for( auto foundUrl : urlList ) {
        reply("checkUrl", foundUrl, depth);
    }

    // Finish execution and die
    reply("done");
    kill();

}
