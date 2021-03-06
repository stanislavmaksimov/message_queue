#ifndef IMESSAGEQUEUEEVENTS_H
#define IMESSAGEQUEUEEVENTS_H

class IMessageQueueEvents
{
public:
    virtual void on_start() = 0;
    virtual void on_stop() = 0;
    virtual void on_hwm() = 0;
    virtual void on_lwm() = 0;
};

#endif // IMESSAGEQUEUEEVENTS_H
