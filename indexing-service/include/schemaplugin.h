#ifndef SCHEMAPLUGIN_H
#define SCHEMAPLUGIN_H

#include <cstring>
#include <optional>
#include <vector>
//#include <QtCore/QIODevice>
#include <QIODevice>
#include <QBuffer>
#include <QUrl>

class SchemaPlugin {
public:
    struct FetchInfo {
        QIODevice* content;
        std::string contentType;
    };

    SchemaPlugin() = default;
    virtual ~SchemaPlugin() = default;

    virtual std::optional<FetchInfo> fetchUrl(const std::string& url) {
        return{};
    }

    virtual std::vector<std::string> schemaSupported () {
        return {};
    }




    virtual QList<QUrl> extractUrls(const QUrl &url, QIODevice *type) {
        return {};
    }

    virtual std::vector<std::string> fileSupported () {
        return {};
    }

    virtual bool isSchema() {
        return false;
    }


};

extern "C" SchemaPlugin* loadPlugin();

#endif // SCHEMAPLUGIN_H
