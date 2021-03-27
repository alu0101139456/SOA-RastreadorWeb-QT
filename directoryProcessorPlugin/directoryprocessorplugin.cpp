#include "directoryprocessorplugin.h"

#include <iostream>

DirectoryProcessorPlugin::DirectoryProcessorPlugin()
{
}

QList<QUrl> DirectoryProcessorPlugin::extractUrls(const QUrl &url, QIODevice *type)
{
    // Go back to the beginning of the stream
    type->seek(0);
    QList<QUrl> urlList;
    QString file;

    // Read all entries in the stream (one per line)
    while ( !(file = type->readLine()).isNull()) {

        // Format data.
        //  - Remove endline
        //  - Add / if none presen
        //  - Add urlType to the beggining
        file.remove('\n');
        QString path(url.path());
        if(path.back() != '/')
        {
            path.append('/');
        }
        QUrl qurl(path + file);
        qurl.setScheme("file");

        // Add url to list
        urlList.push_back(qurl);
    }
    return urlList;
}

std::vector<std::string> DirectoryProcessorPlugin::fileSupported()
{
    static std::vector<std::string> dirtype = {
        "inode/directory"
    };
    return dirtype;

}

SchemaPlugin* loadPlugin() {
    return new DirectoryProcessorPlugin;
}


