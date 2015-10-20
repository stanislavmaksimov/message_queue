#include <QCoreApplication>
#include <QThreadPool>
#include <iostream>
#include <thread>
#include <memory>
#include "messagequeue.h"
#include "messagequeuestd.h"
#include "messagequeuereader.h"
#include "messagequeuewriter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QThreadPool::globalInstance()->setMaxThreadCount(8);

    // qt-queue
    //MessageQueue queue(256, 32, 224);

    // std-queue
    MessageQueueStd queue(256, 32, 224);

    // writer in thread-pool
    QThreadPool::globalInstance()->start(new MessageQueueWriter(queue));

    // writer in std-thread
    std::shared_ptr<MessageQueueWriter> writer = std::shared_ptr<MessageQueueWriter>(new MessageQueueWriter(queue));
    std::thread standardWriterThread(&MessageQueueWriter::run, writer);
    Q_UNUSED(standardWriterThread);

    // reader in thread-pool
    QThreadPool::globalInstance()->start(new MessageQueueReader(queue));

    // reader in std-thread
    std::shared_ptr<MessageQueueReader> reader = std::shared_ptr<MessageQueueReader>(new MessageQueueReader(queue));
    std::thread standardReaderThread(&MessageQueueReader::run, reader);
    Q_UNUSED(standardReaderThread);

    queue.run();

    char someNoneWhitespaceInput;
    std::cin >> someNoneWhitespaceInput;

    queue.stop();

    standardReaderThread.join();
    standardWriterThread.join();
    QThreadPool::globalInstance()->waitForDone();

    return 0;
}
