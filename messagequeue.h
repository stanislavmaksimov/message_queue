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
    explicit MessageQueue(int queue_size, int lwm, int hwm);
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

    const int mQueueSize;
    const int mQueueLwm;
    const int mQueueHwm;
    bool mIsStopped;
    bool mIsHwm;
    bool mIsLwm;
    bool mIsEmpty;
    bool mIsFull;

    std::set<IMessageQueueEvents*> mEventHandlers;
    std::priority_queue<MessageTypeItem> mQueue;
    QMutex mMutex;
    QWaitCondition mNotEmptyOrStoppedWaitCondition;
};

#endif // MESSAGEQUEUE_H
