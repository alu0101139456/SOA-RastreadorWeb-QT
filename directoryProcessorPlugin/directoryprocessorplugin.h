#ifndef DIRECTORYPROCESSORPLUGIN_H
#define DIRECTORYPROCESSORPLUGIN_H

#include <schemaplugin.h>

// Plugin in charge of retrieving files in the directory input in buffer
class DirectoryProcessorPlugin : public SchemaPlugin
{
public:
    DirectoryProcessorPlugin();

    // Retrieves files and directories found in url
    virtual QList<QUrl> extractUrls(const QUrl &url, QIODevice *type);

    // Indicates files supported by this plugin <inode/directory>
    virtual std::vector<std::string> fileSupported ();
};

#endif // DIRECTORYPROCESSORPLUGIN_H
