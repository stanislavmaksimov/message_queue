#include <QCoreApplication>
#include <QThreadPool>
#include <iostream>
#include "messagequeue.h"
#include "messagequeuereader.h"
#include "messagequeuewriter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThreadPool::globalInstance()->setMaxThreadCount(8);

    MessageQueue queue(16, 2, 14);

    QThreadPool::globalInstance()->start(new MessageQueueWriter(queue));
    QThreadPool::globalInstance()->start(new MessageQueueWriter(queue));
    QThreadPool::globalInstance()->start(new MessageQueueWriter(queue));
    QThreadPool::globalInstance()->start(new MessageQueueWriter(queue));

    QThreadPool::globalInstance()->start(new MessageQueueReader(queue));
    QThreadPool::globalInstance()->start(new MessageQueueReader(queue));
    QThreadPool::globalInstance()->start(new MessageQueueReader(queue));
    //QThreadPool::globalInstance()->start(new MessageQueueReader(queue));

    queue.run();

    char someNoneWhitespaceInput;
    std::cin >> someNoneWhitespaceInput;

    queue.stop();
    QThreadPool::globalInstance()->waitForDone();

    return 0;
}
