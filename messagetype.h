#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

#include <string>

typedef std::string MessageType;

struct MessageTypeQueueItem
{
    int priority;
    MessageType message;

    MessageTypeQueueItem(const MessageType& message = MessageType(), int priority = 0)
        : priority(priority)
        , message(message) {}

    bool operator <(const MessageTypeQueueItem& other) const
    {
        return priority < other.priority;
    }
};

#endif // MESSAGETYPE_H
