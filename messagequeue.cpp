#include "messagequeue.h"
#include "imessagequeueevents.h"
#include "logging.h"
#include <QMutexLocker>

MessageQueue::MessageQueue(int queue_size, int lwm, int hwm)
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

MessageQueue::~MessageQueue()
{
    this->stop();
    LOG << this->mQueueSize << this->mQueueLwm << this->mQueueHwm;
}

RetCodes MessageQueue::put(const MessageType& message, int priority)
{
    QMutexLocker locker(&this->mMutex);
    Q_UNUSED(locker);

    if (this->mIsStopped) {
        return STOPPED;
    }
    if (this->mIsFull) {
        return NO_SPACE;
    }

    this->mQueue.push(MessageTypeItem(message, priority));

    this->updateQueueState();
    if (this->mIsHwm) {
        return HWM;
    }
    return OK;
}

RetCodes MessageQueue::get(MessageType& message)
{
    QMutexLocker locker(&this->mMutex);
    Q_UNUSED(locker);

    while (this->mIsEmpty && !this->mIsStopped) {
        this->mNotEmptyOrStoppedWaitCondition.wait(&this->mMutex);
    }

    if (this->mIsStopped) {
        return STOPPED;
    }

    const MessageTypeItem item = this->mQueue.top();
    this->mQueue.pop();
    message = item.message;

    this->updateQueueState();
    if (this->mIsHwm) {
        return HWM;
    }
    return OK;
}

void MessageQueue::stop()
{
    this->setIsStopped(true);
}

void MessageQueue::run()
{
    this->setIsStopped(false);
}

void MessageQueue::set_events(IMessageQueueEvents& events)
{
    this->mEventHandlers.insert(&events);
}

void MessageQueue::setIsStopped(bool value)
{
    if (this->mIsStopped != value) {
        this->mIsStopped = value;
        if (this->mIsStopped) {
            this->mNotEmptyOrStoppedWaitCondition.wakeAll();
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

void MessageQueue::setIsHwm(bool value)
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

void MessageQueue::setIsLwm(bool value)
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

void MessageQueue::setIsEmpty(bool value)
{
    if (this->mIsEmpty != value) {
        this->mIsEmpty = value;
        if (!this->mIsEmpty) {
            this->mNotEmptyOrStoppedWaitCondition.wakeAll();
        }
    }
}

void MessageQueue::setIsFull(bool value)
{
    if (this->mIsFull != value) {
        this->mIsFull = value;
    }
}

void MessageQueue::updateQueueState()
{
    const int size = this->mQueue.size();
    this->setIsFull(size >= this->mQueueSize);
    this->setIsHwm(size >= this->mQueueHwm);
    this->setIsLwm(size <= this->mQueueLwm);
    this->setIsEmpty(size == 0);
}
