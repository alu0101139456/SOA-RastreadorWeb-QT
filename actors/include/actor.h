#ifndef ACTOR_H
#define ACTOR_H

#include <memory>
#include <QObject>
#include <QThread>
#include <QVariant>
#include <functional>
//#include "actorthread.h"
#include <actors_global.h>
#include <actormanager.h>

//extra
#include <QRandomGenerator>
#include <QElapsedTimer>
#include <chrono>

class ActorThread;
class ActorManager;

template <typename Message>
class MailBox;

class ACTORS_EXPORT Actor : public QObject
{
    Q_OBJECT

    // Actor timer structure. it holds:
    //  - timeLeft: time left for timer to fire
    //  - maxTime:  time set to the timer
    //  - message: message to send to actor once timer fires
    struct ActorTimer {
        qint64 timeLeft;
        qint64 maxTime;
        QString message;
//        bool stop;
    };

public:
    using ActorMessage = std::function<void ()>;
    using ActorMailBox = MailBox<ActorMessage>;

    ~Actor();


protected:

    Actor* sender() { return sender_; }
    explicit Actor(Actor* parent);

    // Sends message to execute in receiver actor
    bool send (Actor* receiver, const QString& message, const QVariant& arg0 = QVariant(),
               const QVariant& arg1 = QVariant(), const QVariant& arg2 = QVariant(), const QVariant& arg3 = QVariant(),
               const QVariant& arg4 = QVariant(), const QVariant& arg5 = QVariant(), const QVariant& arg6 = QVariant(),
               const QVariant& arg7 = QVariant(), const QVariant& arg8 = QVariant(), const QVariant& arg9 = QVariant());

    // Replies to sender actor
    bool reply(const QString& message, const QVariant& arg0 = QVariant(),
               const QVariant& arg1 = QVariant(), const QVariant& arg2 = QVariant(), const QVariant& arg3 = QVariant(),
               const QVariant& arg4 = QVariant(), const QVariant& arg5 = QVariant(), const QVariant& arg6 = QVariant(),
               const QVariant& arg7 = QVariant(), const QVariant& arg8 = QVariant(), const QVariant& arg9 = QVariant());

    // Delivery message from sender
    bool delivery_from (Actor* sender , const QString& message, const QVariant& arg0 = QVariant(),
                        const QVariant& arg1 = QVariant(), const QVariant& arg2 = QVariant(), const QVariant& arg3 = QVariant(),
                        const QVariant& arg4 = QVariant(), const QVariant& arg5 = QVariant(), const QVariant& arg6 = QVariant(),
                        const QVariant& arg7 = QVariant(), const QVariant& arg8 = QVariant(), const QVariant& arg9 = QVariant());

    template <typename ActorClass>
    ActorClass* spawn();

    // return id of timer and message to be sent to self when timer runs out.
    //        -1 if it couldnt create it?
    int startTimer(const QString& message, qint64 timeout);

    // add default timeout = -1 if we want to restart timer to max time?
    //     it needs to change data structure so it takes timer and timeout time
    // return true if timer id was found
    // We dont create a timer here since there's no message (could be included)
    bool restartTimer(int id, qint64 timeout = -1);
    bool stopTimer(int id);


    void kill();

public Q_SLOTS:
    // Messaged sent to actor in case no slot is found in receiver
    void unknownMessage(const QString& message);

private:
    bool done_ = false;

    // Stores addr of last actor it recieved a msg from
    Actor* sender_ = nullptr;

    // Thread to execute message
    ActorThread*  thread_;

    // Stores all the messages recieved until they are processed
    std::unique_ptr<ActorMailBox> mailbox_;

    // Store timers currently in use
    QMap<int, ActorTimer> currentTimers_;

    bool processMessage();

    friend ActorThread;
    friend ActorManager;

};

template <typename ActorClass>
ActorClass *Actor::spawn()
{
    return ActorManager::instance()->spawn<ActorClass>(this);
}

#endif // ACTOR_H
