#ifndef SPIWORKER_H
#define SPIWORKER_H

#include <QThread>
#include "ringbuffer.h"


class spiWorker : public QObject {
    Q_OBJECT

public:
    spiWorker(RingBuffer *spiDataBuffer);
    ~spiWorker();

public slots:
    void process();
    
private:
    RingBuffer *pBuffer;

};

#endif // SPIWORKER_H
