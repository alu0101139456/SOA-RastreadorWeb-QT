#include <QCoreApplication>
#include <iostream>
#include <actormanager.h>
#include "getter.h"
#include "indexingservice.h"
#include "testcrawlserver.h"
#include "testlinkchecker.h"

const auto REMOTE_OBJECT_NAME = QStringLiteral("local:indexing-service");
const auto REMOTE_OBJECT_REGISTRY = QStringLiteral("local:registry");

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QRemoteObjectRegistryHost registryNode (REMOTE_OBJECT_REGISTRY);
    QRemoteObjectHost indexingServiceNode (REMOTE_OBJECT_NAME, REMOTE_OBJECT_REGISTRY);

    ActorManager::makeActorManager();
    IndexingService indexingService;

    indexingServiceNode.enableRemoting(&indexingService);


    //Temporal. Used to test linkchecker working
//    ActorManager::makeActorManager(&a);
////    TestLinkChecker* testLinkChecker = ActorManager::spawn<TestLinkChecker>();
//    TestCrawlServer* server = ActorManager::spawn<TestCrawlServer>();
//    QObject::connect(server, SIGNAL(destroyed()), QApplication::instance(), SLOT(quit()));
    return a.exec();
}
