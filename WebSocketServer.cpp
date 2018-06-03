#include "WebSocketServer.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include "mysql.h"


QT_USE_NAMESPACE

void WebSocketServer::parseMessage(QString message, QJsonArray &arr) {
    
    mySQLhandler sql;
    sql.initDBconnection();
    int var = 0;
    if(message == "start") {
        var = 10;
    } else if(message == "init") {
        var = 20;
    } else {
        var = 30;
    }
    switch(var) {
        case 10:
            while(!pBuffer->empty) {
                arr.push_back(pBuffer->pop());
            }
        break;
        case 20:
            arr = sql.getTotalConsumption();
        break;
        case 30:
            arr = sql.getDayConsumption(message);
        break;
    }
}


WebSocketServer::WebSocketServer(quint16 port, RingBuffer *spiDataBuffer, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Echo Server"),
                                            QWebSocketServer::NonSecureMode, this))
{
    pBuffer = spiDataBuffer;
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &WebSocketServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WebSocketServer::closed);
    }
}

WebSocketServer::~WebSocketServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void WebSocketServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

    m_clients << pSocket;
}

void WebSocketServer::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    QJsonObject sendBuffer;
    QJsonArray sendArray;
    parseMessage(message, sendArray);
    QJsonDocument doc(sendArray);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    if (pClient) {
            pClient->sendTextMessage(strJson);
    }
}

void WebSocketServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
