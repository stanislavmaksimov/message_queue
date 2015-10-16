#ifndef IMESSAGEQUEUE_H
#define IMESSAGEQUEUE_H

#include "messagetype.h"
#include "retcodes.h"

class IMessageQueueEvents;

class IMessageQueue
{
public:
    virtual RetCodes put(const MessageType& message, int priority) = 0;
    virtual RetCodes get(MessageType& message) = 0;

    virtual void stop() = 0;
    virtual void run() = 0;

    virtual void set_events(IMessageQueueEvents& events) = 0;
};

#endif // IMESSAGEQUEUE_H
