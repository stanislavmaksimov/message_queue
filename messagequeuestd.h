#ifndef MESSAGEQUEUESTD_H
#define MESSAGEQUEUESTD_H

#include <queue>
#include <set>
#include <mutex>
#include <condition_variable>
#include "imessagequeue.h"
#include "messagetype.h"
#include "retcodes.h"

class IMessageQueueEvents;

class MessageQueueStd : public IMessageQueue
{
public:
    explicit MessageQueueStd(size_t queue_size, size_t lwm, size_t hwm);
    virtual ~MessageQueueStd();

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
    std::mutex mMutex;
    std::condition_variable mNotEmptyOrStoppedWaitCondition;
};

#endif // MESSAGEQUEUESTD_H
