#include "fileschemaplugin.h"

#include <iostream>
#include <QTextStream>
#include <QDir>
#include <QFile>

FileSchemaPlugin::FileSchemaPlugin()
{
}

std::optional<SchemaPlugin::FetchInfo> FileSchemaPlugin::fetchUrl(const std::string &url)
{
    // Create necessary data structures
    QUrl qurl(QString(url.c_str()));
    FetchInfo info;
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForUrl(qurl);
    std::cout << "Path: " << qurl.path().toStdString() << '\n';
    std::cout << "Type: " << mime.name().toStdString() << '\n';


    if( mime.name() == "inode/directory") {

        // Open stream
        info.content = new QBuffer();
        info.content->open(QIODevice::ReadWrite);
        QDir directory(qurl.path());

        // If directory is valid dump all entries into stream
        if(!directory.exists()) {
            return {};
        }
        QTextStream text(info.content);
        QStringList listDir = directory.entryList(QDir::NoDotAndDotDot | QDir::AllEntries);

        foreach (auto fileName, listDir) {
            text << fileName << '\n';
        }
    } else if( mime.name() == "text/html") {
        // Open stream
        info.content = new QBuffer();
        info.content->open(QIODevice::ReadWrite);

        // If file is valid and opened dump all file info into stream and close it
        QFile file(qurl.path());
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream text(info.content);
            text << file.readAll().data();
            file.close();
        } else {
            std::cout << "Couldnt open file\n";
        }

    } else {
        return {};
    }

    // Set content type for processor plugin
    info.contentType = mime.name().toStdString();
    return info;
}

std::vector<std::string> FileSchemaPlugin::schemaSupported()
{
    static std::vector<std::string> schema = {
        "file"
    };
    return schema;
}

SchemaPlugin* loadPlugin() {
    return new FileSchemaPlugin;
}
