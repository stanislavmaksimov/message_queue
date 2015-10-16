#ifndef LOGGING_H
#define LOGGING_H

#include <QDebug>
#include <QThread>

#define LOG qDebug() << QThread::currentThreadId() << Q_FUNC_INFO
#define WRN qWarning() << QThread::currentThreadId() << Q_FUNC_INFO

#endif // LOGGING_H
