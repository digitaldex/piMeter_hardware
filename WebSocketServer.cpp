#include "WebSocketServer.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>


QT_USE_NAMESPACE

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
    while(!pBuffer->empty) {
        sendArray.push_back(pBuffer->pop());
    }
    QJsonDocument doc(sendArray);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    if (pClient) {
        if (message == "start") {
            pClient->sendTextMessage(strJson);
        }
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
