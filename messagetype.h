#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

#include <string>

typedef std::string MessageType;

struct MessageTypeItem
{
    int priority;
    MessageType message;

    MessageTypeItem(const MessageType& message = MessageType(), int priority = 0)
        : priority(priority)
        , message(message) {}

    bool operator <(const MessageTypeItem& other) const
    {
        if (priority != other.priority) {
            return priority < other.priority;
        }
        return message < other.message;
    }
};

#endif // MESSAGETYPE_H
