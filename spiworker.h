#ifndef SPIWORKER_H
#define SPIWORKER_H

#include <unistd.h>
#include <stdint.h>
#include <QThread>
#include <time.h>
#include <thread>
#include <iostream>
#include <bcm2835.h>
#include <stdio.h>
#include "spi.h"
#include "mysql.h"
#include <sys/time.h>
#include <math.h>
#include <string>
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

#endif
