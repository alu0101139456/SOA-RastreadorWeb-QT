#ifndef MAILBOX_H
#define MAILBOX_H

#include <queue>
#include <optional>
#include <mutex>
#include <condition_variable>

// Thread safe message queue
template <typename Message>
class MailBox

{
private:

    std::queue<Message> mailbox_;
    std::mutex mutex_;
    std::condition_variable mailboxNoEmpty_;

public:
    using queue = std::queue<Message>;

    MailBox() = default;

    void push (const Message& msg);
    void push (Message&& msg);
    std::optional<Message> pop( const std::chrono::milliseconds& rel_time);

    void clear();

    unsigned size () const;
    bool isEmpty() const;
};

template<typename Message>
void MailBox<Message>::push(const Message &msg)
{
    std::unique_lock<std::mutex> lock(mutex_);
    mailbox_.push(msg);
    mailboxNoEmpty_.notify_one();
}

template<typename Message>
void MailBox<Message>::push(Message &&msg)
{
    std::unique_lock<std::mutex> lock(mutex_);
    mailbox_.push(std::move(msg));
    mailboxNoEmpty_.notify_one();
}

template<typename Message>
std::optional<Message> MailBox<Message>::pop(const std::chrono::milliseconds& rel_time)
{
    std::unique_lock<std::mutex> lock(mutex_);
    bool noEmpty = mailboxNoEmpty_.wait_for(lock, rel_time, [this] {
        return !mailbox_.empty();
    });
    if(noEmpty) {
        auto message = std::move(mailbox_.front());
        mailbox_.pop();
        return message;
    }
    return {};
}

template<typename Message>
void MailBox<Message>::clear()
{
    std::unique_lock<std::mutex> lock(mutex_);
    mailbox_.swap(queue());
}

template<typename Message>
unsigned MailBox<Message>::size() const
{
    std::unique_lock<std::mutex> lock(mutex_);
    return mailbox_.size();
}

template<typename Message>
bool MailBox<Message>::isEmpty() const
{
    std::unique_lock<std::mutex> lock(mutex_);
    return mailbox_.empty();
}

#endif // MAILBOX_H
