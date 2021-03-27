#ifndef TESTACTOR_H
#define TESTACTOR_H

#include <actor.h>

class TestActor : public Actor
{
    Q_OBJECT
    Q_PROPERTY(int intProperty MEMBER intProperty_);
    Q_PROPERTY(int childHasFailed MEMBER childHasFailed_);
    Q_PROPERTY(Actor* senderProperty MEMBER senderProperty_);
    Q_PROPERTY(int numberMessagesReplied MEMBER numberMessagesReplied_);
    Q_PROPERTY(bool unknownMessage MEMBER unknownMessage_);


public:
    TestActor(Actor* parent) : Actor(parent) {}

    bool testSend (Actor* receiver, const QString& message, const QVariant& arg) {
        return send(receiver, message, arg);
    }




    bool testSend (Actor* receiver, const QString& message) {
        return send(receiver, message);
    }

    Actor* testSender()
    {
        return sender();
    }

    TestActor* spawnChildAndFail() {
        auto child = spawn<TestActor>();
        send(child, "failTask");
        return child;
    }



public Q_SLOTS:
    void updateIntProperty(int arg) {
        intProperty_ = arg;
    }


    void die () {
        kill();
    }

    void saveLastSender()
    {
        senderProperty_ = sender();
    }

    void replyToMessage() {
        reply("messageReceived");
    }

    void failTask(){
        throw 10;
    }

    void failed(){
        childHasFailed_ = true;
    }

    void messageReceived() {
        numberMessagesReplied_++;
    }

    void unknownMessage(const QString& message) {
//        unknownMessage_ = message;
        unknownMessage_ = true;
    }



private:
    int numberMessagesReplied_ = 0;
    int intProperty_;
    bool childHasFailed_ = false;
    Actor *senderProperty_ = nullptr;
    bool unknownMessage_ = false;
};

#endif // TESTACTOR_H
