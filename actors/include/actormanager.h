#ifndef ACTORMANAGER_H
#define ACTORMANAGER_H

#include <QVariant>
#include <QApplication>

#include "actors_global.h"

class Actor;


// Singleton class in charge of creating new actors and managing them in a higher level
class ACTORS_EXPORT ActorManager : public QObject
{
    Q_OBJECT
public:

    // Create and retrieve instance of actormanager
    static ActorManager* makeActorManager (QObject* parent = nullptr);
    static ActorManager* instance();
    ~ActorManager();

    // Create a new Actor
    template <typename ActorClass>
    static ActorClass* spawn (Actor* parent = nullptr);

    // Manager is able to kill actors
    static void kill (Actor* actor);

    // Send a message from manager to actor
    static bool send (Actor* receiver, const QString& message, const QVariant& arg0 = QVariant(),
               const QVariant& arg1 = QVariant(), const QVariant& arg2 = QVariant(), const QVariant& arg3 = QVariant(),
               const QVariant& arg4 = QVariant(), const QVariant& arg5 = QVariant(), const QVariant& arg6 = QVariant(),
               const QVariant& arg7 = QVariant(), const QVariant& arg8 = QVariant(), const QVariant& arg9 = QVariant());

private:
    // Hidden constructor to force instance usage
    explicit ActorManager(QObject *parent = nullptr);

    static ActorManager* instance_;
    QScopedPointer<Actor, QScopedPointerDeleteLater> rootActor_;
};

// Create a new actor and set its parent
template <typename ActorClass>
ActorClass *ActorManager::spawn(Actor *parent)
{
    static_assert(std::is_base_of<Actor, ActorClass>::value);
    ActorClass* newActor = new ActorClass(nullptr);
    newActor->moveToThread(QApplication::instance()->thread());
    newActor->setParent(parent == nullptr ? instance_->rootActor_.get() : parent );

    return newActor;
}

#endif // ACTORMANAGER_H
