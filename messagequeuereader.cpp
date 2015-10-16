#include "messagequeuereader.h"
#include "imessagequeue.h"
#include "logging.h"
#include <iostream>

MessageQueueReader::MessageQueueReader(IMessageQueue& queue)
    : mQueue(queue)
{
    LOG;
}

MessageQueueReader::~MessageQueueReader()
{
    LOG;
}

void MessageQueueReader::run()
{
    LOG;
    MessageType message;
    RetCodes result = this->mQueue.get(message);
    while (result != STOPPED) {
        switch (result) {
        case OK:
        case HWM:
            this->handle_message(message);
            break;
        default:
            break;
        }
        result = this->mQueue.get(message);
    }
}

void MessageQueueReader::handle_message(const MessageType& message)
{
    qDebug() << "message handled in" << (quint64)QThread::currentThreadId() << "message:" << QString::fromStdString(message);
}
