#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include "spiworker.h"
#include "ringbuffer.h"
#include "WebSocketServer.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    RingBuffer spiDataBuffer;
    qint16 port = 8888;
    WebSocketServer *server = new WebSocketServer(port, &spiDataBuffer);
    QObject::connect(server, &WebSocketServer::closed, &a, &QCoreApplication::quit);

    QThread *Thread = new QThread;
    spiWorker *worker = new spiWorker(&spiDataBuffer);
    worker->moveToThread(Thread);
    QObject::connect(Thread, SIGNAL (started()), worker, SLOT (process()));
    Thread->start();

    return a.exec();
}
