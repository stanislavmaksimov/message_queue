#ifndef MESSAGEQUEUEWRITER_H
#define MESSAGEQUEUEWRITER_H

#include <QRunnable>
#include <QWaitCondition>
#include "imessagequeueevents.h"
#include "messagetype.h"

class IMessageQueue;

class MessageQueueWriter : public QRunnable, public IMessageQueueEvents
{
public:
    explicit MessageQueueWriter(IMessageQueue& queue);
    virtual ~MessageQueueWriter();

    void run();

    void on_start() {}
    void on_stop();
    void on_hwm() {}
    void on_lwm();

private:
    static MessageType generate(quint64 threadId, int& priority);

    IMessageQueue& mQueue;
    QWaitCondition mLwmOrStopped;
};

#endif // MESSAGEQUEUEWRITER_H
