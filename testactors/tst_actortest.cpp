#include <QtTest>
#include <QCoreApplication>

#include "testactor.h"
#include <actormanager.h>


class actortest : public QObject
{
    Q_OBJECT

public:
    actortest();
    ~actortest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void slotIsCalledWhenMessageIsSend();
    void actorKillItSelfWhenIsRequest();
    void actorIsNotifiedWhenChildFails();
    //                   CREAR CLASES TESTEO POR CLASES A TESTEAR
    void senderIsNullWhenNoMessage();
    void senderIsSetWhenMessageArrives();

    void canReplyMessage();
    void unknownMessageSlotWorks();


//    Actor::unknownMessage(const QString& message);
    // Child Failure
    // Reply
    // Actor dies


    /* TODO:
    void slotCanReplyToSenderActor();

//
    void slotIsCalledWhenUnkownMessage();

    Actor::kill(); ActorManager::kill(Actor* actor);



    OPCIONAL --> Testing del ActorManager
    */


private:
    TestActor* actorA_;
    TestActor* actorB_;

};

actortest::actortest()
{

}

actortest::~actortest()
{

}

void actortest::initTestCase()
{
    ActorManager::makeActorManager(this);
    actorA_ = ActorManager::instance()->spawn<TestActor>();
    actorB_ = ActorManager::instance()->spawn<TestActor>();
}

void actortest::cleanupTestCase()
{
//    QPointer<TestActor> p = actorA_;

    ActorManager::kill(actorA_);
    ActorManager::kill(actorB_);

//    while(!p.isNull());
}

void actortest::slotIsCalledWhenMessageIsSend()
{
    int arg = 10;
    QVERIFY(actorA_->testSend(actorB_, "updateIntProperty", arg));
    QTest::qWait(100); //hay que tener siempre en cuenta el tiempo porque son hilos y estan vivos
    QVERIFY(actorB_->property("intProperty").canConvert<int>());
    QCOMPARE(actorB_->property("intProperty").toInt(), arg);
}


void actortest::senderIsNullWhenNoMessage()
{
    QCOMPARE(actorA_->testSender(), nullptr);
}

void actortest::senderIsSetWhenMessageArrives()
{
    // sender is not set before we set the message
    QVERIFY(actorB_->property("senderProperty").isNull());
    actorA_->testSend(actorB_, "saveLastSender");
    QTest::qWait(100);
    // sender is not null when the message arrives
    QVERIFY(!actorB_->property("senderProperty").isNull());
    // sender is null after attending the message
    QCOMPARE(actorB_->testSender(), nullptr);
}


void actortest::actorKillItSelfWhenIsRequest() {
    QPointer<TestActor> actor = ActorManager::instance()->spawn<TestActor>();

    QVERIFY(actor->testSend(actor, "die"));
    QTest::qWait(100);
    QVERIFY(actor.isNull());
}

void actortest::canReplyMessage() {

    QCOMPARE(actorA_->property("numberMessagesReplied").toInt(), 0);
    actorA_->testSend(actorB_, "replyToMessage");
    QTest::qWait(100);
    QCOMPARE(actorA_->property("numberMessagesReplied").toInt(), 1);


}

void actortest::actorIsNotifiedWhenChildFails()
{
    QVERIFY(!actorA_->property("childHasFailed").toBool());
    auto child = actorA_->spawnChildAndFail();

    QTest::qWait(100);
    auto cleanup = qScopeGuard([child]() {
        ActorManager::kill(child);
    });

    // Comprueba si actorA_ recibio el mensaje de fail del hijo
    QVERIFY(actorA_->property("childHasFailed").toBool());


}

void actortest::unknownMessageSlotWorks() {
//    QCOMPARE(actorA_->property("unknownMessage").toString(), "");
    QVERIFY(!actorA_->property("unknownMessage").toBool());

    actorB_->testSend(actorA_, "unknow");
    QTest::qWait(100);
     QVERIFY(actorA_->property("unknownMessage").toBool());
//    QCOMPARE(actorA_->property("unknownMessage").toString(), "unknow");
}

QTEST_MAIN(actortest)

#include "tst_actortest.moc"
