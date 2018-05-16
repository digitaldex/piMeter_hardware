#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <QObject>
#include <QVector>
#include <QJsonObject>
#include <QMutex>

using namespace std;

class RingBuffer {
private:
        int head = 0;
        int tail = 0;
        const static int size = 600;
        QJsonObject data[size];
        QMutex m;
public:
    //RingBuffer();
    bool empty = true;
    void push(QJsonObject spiData) {
        m.lock();
        data[head] = spiData;
        head = (head + 1) % size;
        m.unlock();
        checkEmpty();
    }
    QJsonObject pop() {
        if(empty == false) {
            m.lock();
            QJsonObject ret = data[tail];
            tail = (tail + 1) % size;
            m.unlock();
            checkEmpty();
            return ret;
        }
    }
    void checkEmpty() {
        if (head == tail) {
            empty = true;
        } else {
            empty = false;
        }
    }
};

#endif // RINGBUFFER_H
