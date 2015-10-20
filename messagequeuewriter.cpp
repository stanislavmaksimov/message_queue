#include "messagequeuewriter.h"
#include "imessagequeue.h"
#include "logging.h"
#include <QDateTime>
#include <QAtomicInt>
#include <QMutex>

MessageType MessageQueueWriter::generate(quint64 threadId, int& priority)
{
    static QAtomicInt INDEX = 0;

    priority = qrand() % 10;

    return QString("index: %1; priority: %2; thread: %3")
            .arg(++INDEX)
            .arg(priority)
            .arg(threadId).toStdString();
}

MessageQueueWriter::MessageQueueWriter(IMessageQueue& queue)
    : mQueue(queue)
    , mLwmOrStopped()
{
    LOG;
    queue.set_events(*this);
}

MessageQueueWriter::~MessageQueueWriter()
{
    LOG;
}

void MessageQueueWriter::run()
{
    LOG;
    QMutex mutex;

    qsrand(QTime::currentTime().msecsSinceStartOfDay());

    const quint64 threadId = (quint64)QThread::currentThreadId();
    int priority = 0;

    RetCodes result = this->mQueue.put(generate(threadId, priority), priority);
    while (result != STOPPED) {
        switch (result) {
        case HWM:
        case NO_SPACE:
        {
            QMutexLocker locker(&mutex);
            Q_UNUSED(locker);
            this->mLwmOrStopped.wait(&mutex);
            break;
        }
        default:
            break;
        }
        result = this->mQueue.put(generate(threadId, priority), priority);
    }
}

void MessageQueueWriter::on_lwm()
{
    this->mLwmOrStopped.wakeAll();
}

void MessageQueueWriter::on_stop()
{
    this->mLwmOrStopped.wakeAll();
}
