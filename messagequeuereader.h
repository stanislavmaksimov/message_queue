#ifndef MESSAGEQUEUEREADER_H
#define MESSAGEQUEUEREADER_H

#include <QRunnable>
#include "imessagequeueevents.h"
#include "messagetype.h"

class IMessageQueue;

class MessageQueueReader : public QRunnable
{
public:
    explicit MessageQueueReader(IMessageQueue& queue);
    virtual ~MessageQueueReader();

    void run();
    void handle_message(const MessageType&);

private:
    IMessageQueue& mQueue;
};

#endif // MESSAGEQUEUEREADER_H
