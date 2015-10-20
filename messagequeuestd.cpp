#include "messagequeuestd.h"
#include "imessagequeueevents.h"
#include "logging.h"

MessageQueueStd::MessageQueueStd(size_t queue_size, size_t lwm, size_t hwm)
    : mQueueSize(queue_size)
    , mQueueLwm(lwm)
    , mQueueHwm(hwm)
    , mIsStopped(false)
    , mIsHwm(false)
    , mIsLwm(false)
    , mIsEmpty(false)
    , mIsFull(false)
    , mEventHandlers()
    , mQueue()
    , mMutex()
    , mNotEmptyOrStoppedWaitCondition()
{
    LOG << this->mQueueSize << this->mQueueLwm << this->mQueueHwm;
    this->updateQueueState();
}

MessageQueueStd::~MessageQueueStd()
{
    this->stop();
    LOG << this->mQueueSize << this->mQueueLwm << this->mQueueHwm;
}

RetCodes MessageQueueStd::put(const MessageType& message, int priority)
{
    std::unique_lock<std::mutex> locker(this->mMutex);
    Q_UNUSED(locker);

    if (this->mIsStopped) {
        return STOPPED;
    }
    if (this->mIsFull) {
        return NO_SPACE;
    }

    this->mQueue.push(MessageTypeQueueItem(message, priority));

    this->updateQueueState();
    if (this->mIsHwm) {
        return HWM;
    }
    return OK;
}

RetCodes MessageQueueStd::get(MessageType& message)
{
    std::unique_lock<std::mutex> locker(this->mMutex);
    Q_UNUSED(locker);

    while (this->mIsEmpty && !this->mIsStopped) {
        this->mNotEmptyOrStoppedWaitCondition.wait(locker);
    }

    if (this->mIsStopped) {
        return STOPPED;
    }

    const MessageTypeQueueItem item = this->mQueue.top();
    this->mQueue.pop();
    message = item.message;

    this->updateQueueState();
    if (this->mIsHwm) {
        return HWM;
    }
    return OK;
}

void MessageQueueStd::stop()
{
    this->setIsStopped(true);
}

void MessageQueueStd::run()
{
    this->setIsStopped(false);
}

void MessageQueueStd::set_events(IMessageQueueEvents& events)
{
    this->mEventHandlers.insert(&events);
}

void MessageQueueStd::setIsStopped(bool value)
{
    if (this->mIsStopped != value) {
        this->mIsStopped = value;
        if (this->mIsStopped) {
            this->mNotEmptyOrStoppedWaitCondition.notify_all();
        }
        foreach (IMessageQueueEvents* eventHandler, this->mEventHandlers) {
            if (this->mIsStopped) {
                eventHandler->on_stop();
            } else {
                eventHandler->on_start();
            }
        }
    }
}

void MessageQueueStd::setIsHwm(bool value)
{
    if (this->mIsHwm != value) {
        this->mIsHwm = value;
        if (this->mIsHwm) {
            foreach (IMessageQueueEvents* eventHandler, this->mEventHandlers) {
                eventHandler->on_hwm();
            }
        }
    }
}

void MessageQueueStd::setIsLwm(bool value)
{
    if (this->mIsLwm != value) {
        this->mIsLwm = value;
        if (this->mIsLwm) {
            foreach (IMessageQueueEvents* eventHandler, this->mEventHandlers) {
                eventHandler->on_lwm();
            }
        }
    }
}

void MessageQueueStd::setIsEmpty(bool value)
{
    if (this->mIsEmpty != value) {
        this->mIsEmpty = value;
        if (!this->mIsEmpty) {
            this->mNotEmptyOrStoppedWaitCondition.notify_all();
        }
    }
}

void MessageQueueStd::setIsFull(bool value)
{
    if (this->mIsFull != value) {
        this->mIsFull = value;
    }
}

void MessageQueueStd::updateQueueState()
{
    const size_t size = this->mQueue.size();
    this->setIsFull(size >= this->mQueueSize);
    this->setIsHwm(size >= this->mQueueHwm);
    this->setIsLwm(size <= this->mQueueLwm);
    this->setIsEmpty(size == 0);
}
