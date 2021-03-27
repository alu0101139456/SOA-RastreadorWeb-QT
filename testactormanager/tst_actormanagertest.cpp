#include <QtTest>
#include <QCoreApplication>

// add necessary includes here
#include <actormanager.h>
#include <actor.h>
#include "../testactors/testactor.h"

class ActorManagerTest : public QObject
{
    Q_OBJECT

public:
    ActorManagerTest();
    ~ActorManagerTest();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void actorIsSpawnedWhenActorManagerRequestIt();
    void actorIsKilledWhenActorManagerRequestIt();
    void actorManagerSendsAMessage();
};

ActorManagerTest::ActorManagerTest()
{

}

ActorManagerTest::~ActorManagerTest()
{

}

void ActorManagerTest::initTestCase()
{
    ActorManager::makeActorManager();
}

void ActorManagerTest::cleanupTestCase()
{

}

void ActorManagerTest::actorIsSpawnedWhenActorManagerRequestIt()
{
    TestActor* actor = nullptr;
    QPointer<TestActor> actorPointer = actor;
    actor = ActorManager::spawn<TestActor>();
    QTest::qWait(100);
    QVERIFY(actorPointer.isNull());
}

void ActorManagerTest::actorIsKilledWhenActorManagerRequestIt()
{
    Actor* actor = ActorManager::spawn<Actor>();

    QPointer<Actor> actorPointer = actor;

    ActorManager::kill(actor);
    QTest::qWait(100);

    QVERIFY(actorPointer.isNull());
}

void ActorManagerTest::actorManagerSendsAMessage()
{
    auto actorA = ActorManager::spawn<TestActor>();
    int arg = 10;
    QVERIFY(ActorManager::send(actorA, "updateIntProperty", arg));
    QTest::qWait(100); //hay que tener siempre en cuenta el tiempo porque son hilos y estan vivos
    QVERIFY(actorA->property("intProperty").canConvert<int>());
    QCOMPARE(actorA->property("intProperty").toInt(), arg);
}

QTEST_MAIN(ActorManagerTest)

#include "tst_actormanagertest.moc"
