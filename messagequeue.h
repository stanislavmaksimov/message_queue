#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <queue>
#include <set>
#include "imessagequeue.h"
#include "messagetype.h"
#include "retcodes.h"

#include <QMutex>
#include <QWaitCondition>

class QReadWriteLock;
class IMessageQueueEvents;

class MessageQueue : public IMessageQueue
{
public:
    explicit MessageQueue(size_t queue_size, size_t lwm, size_t hwm);
    virtual ~MessageQueue();

    RetCodes put(const MessageType& message, int priority);
    RetCodes get(MessageType& message);

    void stop();
    void run();
    void set_events(IMessageQueueEvents& events);

private:
    void setIsStopped(bool);
    void setIsHwm(bool);
    void setIsLwm(bool);
    void setIsEmpty(bool);
    void setIsFull(bool);
    void updateQueueState();

    const size_t mQueueSize;
    const size_t mQueueLwm;
    const size_t mQueueHwm;
    bool mIsStopped;
    bool mIsHwm;
    bool mIsLwm;
    bool mIsEmpty;
    bool mIsFull;

    std::set<IMessageQueueEvents*> mEventHandlers;
    std::priority_queue<MessageTypeQueueItem> mQueue;
    QMutex mMutex;
    QWaitCondition mNotEmptyOrStoppedWaitCondition;
};

#endif // MESSAGEQUEUE_H
